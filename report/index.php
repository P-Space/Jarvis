<?php
include("credentials.php");
$con = mysql_connect($server,$username,$password);
mysql_select_db ($db, $con);
if (!$con) die('Could not connect: ' . mysql_error());
$limit=10;
$button = true;
$style = true;
$json=false;
if(isset($_GET['limit']))
	$limit=$_GET['limit'];

if(isset($_GET['nobutton']))
	$button=false;

if(isset($_GET['nostyle']))
	$style=false;
if(isset($_GET['json']))
	$json=true;

$sql="SELECT * FROM events ";
if(!$button)
	$sql = $sql."WHERE type != 'remote'";
$sql = $sql."ORDER BY timestamp DESC LIMIT ".$limit;
$result=mysql_query($sql);
if (!$result)
	die("Could not access database: ".mysql_error());
else
{
	$events = array();

	if($style && !$json)
	{
		echo "<style type='text/css'><!-- @import url('style.css'); --></style>";
		if(isset($_GET['small']))
			echo "<style type='text/css'><!-- @import url('style-small.css'); --></style>";

		echo "<table id='events'>";
		echo "<tr id = 'header'><th>Time</th><th>Event</th></tr>";
	}
	while($row =mysql_fetch_row($result))
	{
		$event = array();
		$event['type']="check-in";
		if($row[2]=="remote")
		{
			if($json)
			{
				$event['extra']="Remote button pressed!";
				$event['name']="BBoD";
			}
			elseif($style)
				$table_row =  "<td>Remote button pressed!</td>";
			else
				$table_row =  "Remote button pressed!<br />";
		}
		else
		{
			if($json)
			{
				$event['extra']="Card was used by: ".$row[3];
				$event['name']=$row[3];
			}
			elseif($style)
				$table_row = "<td>Card was used by: ".$row[3]."</td>";
			else
				$table_row = $row[3]."<br />";
			
		}
		if($json)
			$event['t']=strtotime($row[1]);
		elseif($style)
		{
			echo "<tr id ='".$row[2]."'><td>".date("M j, G:i", strtotime($row[1]))."</td>";
			echo $table_row;
			echo "</tr>";
		}
		else
			echo strtotime($row[1])." ".$table_row;

		$events[]  = $event;	
	}
	if($json)
	{
		header('Content-type: application/json');
		echo json_encode(array('events'=>$events));
	}
	elseif($style)
		echo "</table>";
	
 }

mysql_close($con);
?>
