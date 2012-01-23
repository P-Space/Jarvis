<?php
include("credentials.php");
$con = mysql_connect($server,$username,$password);
mysql_select_db ($db, $con);
if (!$con) die('Could not connect: ' . mysql_error());
$limit=10;
$button = true;
$style = true;
if(isset($_GET['limit']))
	$limit=$_GET['limit'];

if(isset($_GET['nobutton']))
	$button=false;

if(isset($_GET['nostyle']))
	$style=false;

$sql="SELECT * FROM events ";
if(!$button)
	$sql = $sql."WHERE type != 'remote'";
$sql = $sql."ORDER BY timestamp DESC LIMIT ".$limit;
$result=mysql_query($sql);
if (!$result)
	die("Could not access database: ".mysql_error());
else
{
	if($style)
	{
		echo "<style type='text/css'><!-- @import url('style.css'); --></style>";
		if(isset($_GET['small']))
			echo "<style type='text/css'><!-- @import url('style-small.css'); --></style>";

		echo "<table id='events'>";
		echo "<tr id = 'header'><th>Time</th><th>Event</th></tr>";
	}
	while($row =mysql_fetch_row($result))
	{
		if($row[2]=="remote")
		{
			if($style)
				$table_row =  "<td>Remote button pressed!</td>";
			else
				$table_row =  "Remote button pressed!<br />";
		}
		else
		{
			if($style)
				$table_row = "<td>Card was used by: ".$row[3]."</td>";
			else
				$table_row = $row[3]."<br />";
			
		}
		if($style)
		{
			echo "<tr id ='".$row[2]."'><td>".date("M j, G:i", strtotime($row[1]))."</td>";
			echo $table_row;
			echo "</tr>";
		}
		else
			echo $table_row;			
	}
	if($style)
		echo "</table>";
}

mysql_close($con);
?>
