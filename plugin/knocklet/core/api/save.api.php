<?php
        include_once("../class/knocklet.class.php");

echo "bonjour";

if (isset($_POST['save'])) {
	//$lala = unserialize($_POST['save']);
	//print_r($lala);
	//echo knocklet::saveConfigFromSerialized($_POST['save']);
	echo $_POST['save'];
	chdir("../class/");
	knocklet::saveConfigFromSerialized('a:5:{i:1;a:3:{s:10:"braceletId";s:1:"1";s:8:"moduleId";s:1:"0";s:6:"knocks";s:1:"3";}i:2;a:3:{s:10:"braceletId";s:1:"5";s:8:"moduleId";s:1:"4";s:6:"knocks";s:1:"2";}i:3;a:3:{s:10:"braceletId";s:1:"0";s:8:"moduleId";s:1:"5";s:6:"knocks";s:1:"1";}i:4;a:3:{s:10:"braceletId";s:1:"5";s:8:"moduleId";s:1:"2";s:6:"knocks";s:1:"0";}s:0:"";a:3:{s:10:"braceletId";s:0:"";s:8:"moduleId";s:0:"";s:6:"knocks";s:0:"";}}');
}
	knocklet::saveConfigFromSerialized('a:5:{i:1;a:3:{s:10:"braceletId";s:1:"1";s:8:"moduleId";s:1:"0";s:6:"knocks";s:1:"3";}i:2;a:3:{s:10:"braceletId";s:1:"5";s:8:"moduleId";s:1:"4";s:6:"knocks";s:1:"2";}i:3;a:3:{s:10:"braceletId";s:1:"0";s:8:"moduleId";s:1:"5";s:6:"knocks";s:1:"1";}i:4;a:3:{s:10:"braceletId";s:1:"5";s:8:"moduleId";s:1:"2";s:6:"knocks";s:1:"0";}s:0:"";a:3:{s:10:"braceletId";s:0:"";s:8:"moduleId";s:0:"";s:6:"knocks";s:0:"";}}');


?>




