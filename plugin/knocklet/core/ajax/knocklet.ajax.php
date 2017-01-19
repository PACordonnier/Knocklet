<?php

/* This file is part of Jeedom.
 *
 * Jeedom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Jeedom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Jeedom. If not, see <http://www.gnu.org/licenses/>.
 */

try {
    require_once dirname(__FILE__) . '/../../../../core/php/core.inc.php';
    include_file('core', 'authentification', 'php');

    if (!isConnect('admin')) {
        throw new Exception('401 Unauthorized');
    }

    ajax::init();


if (isset($_POST['saveCmd'])) {
        $js = $_POST['saveCmd'];
        chdir("../class/");
        knocklet::saveCmdConfigFromJson($js);
//TODO AJOUTER CA PARTOUT ET FAIRE QUE CA MARCHE	ajax::succes();
}

if (isser($_POST['saveScio'])) {
	$js = $_POST['saveSCio'];
        chdir("../class/");
        knocklet::saveScioConfigFromJson($js);
}

/*
//Inutile désormais, les modules s'ajoute tout seul au menu dès qu'ils sont détéctés

$syncFile = "/tmp/syncFile";
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
*/
    throw new Exception('Aucune methode correspondante');
    /*     * *********Catch exeption*************** */
} catch (Exception $e) {
    ajax::error(displayExeption($e), $e->getCode());
}
?>



