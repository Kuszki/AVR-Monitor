<?php

require("../../dev/database.php");
require("../../dev/params.php");

$DB = new Database("localhost", "avr_monitor", "kuszkiavrmonitor", "avr_monitor");
$COLORS = ["red", "blue", "green", "darkorange", "crimson", "olive", "sienna"];
$PARAMS = new Params();

$i = 0;

switch ($PARAMS["job"])
{
	case "init":
		echo '{ "labels": [], "datasets": ['."\n";

		foreach ($DB->Query("SELECT `name` FROM `variables`") as $Var)
		{
			$Data = $Data.'{ "label": "'.$Var[0].'", "fillColor": "rgba(0,0,0,0)", "strokeColor": "'.$COLORS[$i].'", "pointColor": "'.$COLORS[$i].'", "data": [] },'; $i++;
		}

		echo trim($Data, ",").'] }';
	break;

	case "pull":
		echo '{ "data": [';

		foreach ($DB->Query("SELECT `value` FROM `variables`") as $Var)
		{
			$Data = $Data.sprintf("%0.3f", $Var[0]).",";
		}

		echo trim($Data, ",").'] }';
	break;
}
?>
