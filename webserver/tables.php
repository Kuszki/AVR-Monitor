<?php

require("../../dev/database.php");
require("../../dev/params.php");

$DB = new Database("localhost", "avr_monitor", "kuszkiavrmonitor", "avr_monitor");
$PARAMS = new Params();

switch ($PARAMS["tab"])
{
	case "system":

		echo '<table id="system" class="variables">'."\n";
		echo '<th>Rejestr</th><th>Stan</th><th>HEX</th>'."\n";

		foreach ($DB->Query("SELECT `name`, `value` FROM `system`") as $Var)
		{
			echo '<tr><td class="name">'.$Var[0].'</td><td class="value">'.$Var[1].'</td><td class="value">0x'.dechex($Var[1]).'</td></tr>'."\n";
		}

		echo '</table>'."\n";

	break;

	case "converters":

		echo '<table id="system" class="variables">'."\n";
		echo '<th>Przetwornik</th><th>Napięcie</th>'."\n";

		foreach ($DB->Query("SELECT `ID`, `value` FROM `converters`") as $Var)
		{
			echo '<tr><td class="name">'."ADC ".$Var[0].'</td><td class="value">'.$Var[1]." V".'</td></tr>'."\n";
		}

		echo '</table>'."\n";

	break;

	case "variables":

		echo '<table id="system" class="variables">'."\n";
		echo '<th>Zmienna</th><th>Wartość</th>'."\n";

		foreach ($DB->Query("SELECT `name`, `value` FROM `variables`") as $Var)
		{
			echo '<tr><td class="name">'.$Var[0].'</td><td class="value">'.$Var[1].'</td></tr>'."\n";
		}

		echo '</table>'."\n";

	break;
}

?>
