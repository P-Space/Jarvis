import serial
import string
import time
import _mysql
import _mysql_exceptions
import re

import config

On = False
Off = True

def ByteToHex( byteStr ):
    """
    Convert a byte string to it's hex string representation e.g. for output.
    """
    
    # Uses list comprehension which is a fractionally faster implementation than
    # the alternative, more readable, implementation below
    #   
    #    hex = []
    #    for aChar in byteStr:
    #        hex.append( "%02X " % ord( aChar ) )
    #
    #    return ''.join( hex ).strip()        

    return ''.join( [ "%02X " % ord( x ) for x in byteStr ] ).strip()

#If the button has been pressed more than 1 second ago, reset its status to "false".
def checkIfReleased():
	global buttonPressed
	if time.time() - lasttime > 1 and buttonPressed is True:
		print "Button Released!"
		buttonPressed = False

#controlDoor(): Releases the door if more than "delay" seconds have passed 
#since the door was last energized ("doortime"). 
def controlDoor():
	global doortime
	delay = 2
	if doortime != 0:
		if time.time() - doortime > delay:
			doortime = 0
			ser.setDTR(Off)
			print "Door released"

ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=0.5)
print ser.portstr 
ser.setDTR(Off)

lasttime = 0
buttonPressed = False
doortime = 0

while True:
	s = ser.read(16)
	ser.flushInput()
	#print ByteToHex(x)
	checkIfReleased()
	controlDoor()
	if len(s) < 14:
		continue
	#if string.find("0x7E", ByteToHex(s[0])) != -1:
	done = 0
	for i in range(len(s)):
		#print(ByteToHex(s[i]))
		if string.find("0x83", ByteToHex(s[i])) != -1:
			if len(s) - i >= 3:
				if string.find("0x00", ByteToHex(s[i+1])) != -1 and string.find("0x03", ByteToHex(s[i+2])) != -1:
					# print "Last Timestamp: "+time.strftime("%H:%M:%S")
					newtime = time.time()
					# print lasttime
					# print newtime
					if newtime - lasttime > 2:
						print "Button Pressed! Timestamp: "+time.strftime("%H:%M:%S")
						try:
							db=_mysql.connect(host="localhost", user=config.USER, passwd=config.PASSWD,db=config.DB)
							db.query("""INSERT INTO events(type) VALUES('remote')""") 
							db.close()
						except _mysql.Error:
							print "Shit! DB Error!"
						except NameError:
							print "No DB was initialized"
						buttonPressed = True
						doortime = newtime
						print "Door opened"
						ser.setDTR(On)
					lasttime = newtime
					done = 1
	if done == 0 and len(s) == 16 and string.find("0x02", ByteToHex(s[0])) != -1 and string.find("0x03", ByteToHex(s[15])) != -1:
		print "ID Card detected!"
		#s = ser.read(15)
		#print s[1:13]
		found = 0
		for line in open("cards.txt"):
			#Check if the line contains the number of the card
			if s[1:13] in line:
				found = 1
				#line format: cardnumber\tusername\n
				username = line.split("\t")[1].split("\n")[0]

				#Check if the card has been cancelled
				if re.match("#", line):
					print "Card " + s[1:13] + " rejected for user " + username
					break
				
				#If the card has been found in the file and is valid, open the door
				print "Card belongs to: " + username
				print "Accepted!"
				newtime = time.time()
				doortime = newtime
				print "Door opened"
				ser.setDTR(On)
				
				try:
					db=_mysql.connect(host="localhost", user=config.USER, passwd=config.PASSWD,db=config.DB)
					db.query("""INSERT INTO events(type, name, card) VALUES('card', '"""+username+"""', '"""+s[1:13]+"""')""") 
					db.close()
				except _mysql.Error:
					print "Shit! DB Error!"
				except NameError:
					print "No DB was initialized"
				break
		if found == 0:
			print "Unknown Card Number: " + s[1:13]
	
	elif done == 0:
		print "Got undefined data: " + s + " Hex: " + ByteToHex(s)
		
	checkIfReleased()
	controlDoor()

	ser.flushInput()
ser.close()
