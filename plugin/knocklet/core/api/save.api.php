<?php
        include_once("../class/knocklet.class.php");

echo "bonjour";

if (isset($_POST['save'])) {
	$js = $_POST['save'];
	chdir("../class/");
	knocklet::saveConfigFromJson($js);
}


?>




