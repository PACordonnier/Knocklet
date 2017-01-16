<?php
include_once("../class/knocklet.class.php");

$syncFile = "/tmp/syncTest";

if (isset($_POST['save'])) {
	$js = $_POST['save'];
	chdir("../class/");
	knocklet::saveConfigFromJson($js);
}


if (isset($_POST['startScan2'])) {

	if (file_exists($syncFile) && (time() - filemtime($syncFile)) > 1000) unlink($syncFile);
	
	echo shell_exec('cat  /tmp/syncTest');
}

//echo shell_exec('exec tail -n50  /tmp/syncTest');
if (isset($_POST['startScan'])) {

	if (file_exists($syncFile) && (time() - filemtime($syncFile)) > 1000) unlink($syncFile);
	$handle = fopen($syncFile, "r");
        if ($handle) {
		$tab = array();
                while (($line = fgets($handle)) !== false) {
                        $line = str_replace("\n","",$line);
			$found = false;
			foreach($array as $value)
				if($value == $line) $found = true;
			if(!$found) $array[]=$line;
                }
                foreach($array as $value) echo $value."\n";

                fclose($handle);
        } else {
            // error opening the file.
        }

}

?>




