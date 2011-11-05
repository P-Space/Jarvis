<?php
include("credentials.php");
$con = mysql_connect($server,$username,$password);
mysql_select_db ($db, $con);
if (!$con) die('Could not connect: ' . mysql_error());
$limit=10;
if(isset($_GET['limit']))
	$limit=$_GET['limit'];

$sql="SELECT * FROM events ORDER BY timestamp DESC LIMIT ".$limit;
$result=mysql_query($sql);
if (!$result)
	die("Could not access database: ".mysql_error());
else
{
	echo "<style type='text/css'><!-- @import url('style.css'); --></style>";
	if(isset($_GET['small']))
		echo "<style type='text/css'><!-- @import url('style-small.css'); --></style>";
	
	echo "<table id='events'>";
	echo "<tr id = 'header'><th>Time</th><th>Event</th></tr>";
	while($row =mysql_fetch_row($result))
	{
		echo "<tr id ='".$row[2]."'><td>".date("M j, G:i", strtotime($row[1]))."</td>";
		if($row[2]=="remote")
		{
			echo "<td>Remote button pressed!</td>";
		}
		else
		{
			echo "<td>Card was used by: ".$row[3]."</td>";
		}
		echo "</tr>";
	}
	echo "</table>";
}

mysql_close($con);
?>
