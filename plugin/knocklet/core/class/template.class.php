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

/* * ***************************Includes********************************* */
require_once dirname(__FILE__) . '/../../../../core/php/core.inc.php';

/*
class knockConvert {
       //  * *************************Attributs****************************** 
        private $braceletId;
        private $moduleId;
        private $knocks;
        private $cmdId;

        function __construct($braceletId,$moduleId,$knocks,$cmdId){
                $this->braceletId=$braceletId;
                $this->moduleId=$moduleId;
                $this->knocks=$knocks;
                $this->cmdId=cmdId;
        }

	function save($file){
		file_put_contents($file, $braceletId . " ". $moduleId . " " . $knocks . " " . cmdId, FILE_APPEND);
	}

}
*/

class template extends eqLogic {
    /*     * *************************Attributs****************************** */
	private $knockArray=array();
	private $configFile="/tmp/knocklet/config";

    /*     * ***********************Methode static*************************** */

    /*
     * Fonction exécutée automatiquement toutes les minutes par Jeedom
      public static function cron() {

      }
     */


    /*
     * Fonction exécutée automatiquement toutes les heures par Jeedom
      public static function cronHourly() {

      }
     */

    /*
     * Fonction exécutée automatiquement tous les jours par Jeedom
      public static function cronDayly() {

      }
     */



    /*     * *********************Méthodes d'instance************************* */
    private function createTriplet($bId,$mId,$knocks) {
	return array("braceletId"=>$bId,"moduleId"=>$mId,"knocks"=>$knocks);

    }
 
    public function load() {/*
	$this->knockArray["a"]=self::createTriplet(1,0,3);
	$this->knockArray["b"]=self::createTriplet(5,4,2);
	$this->knockArray["c"]=self::createTriplet(0,5,1);
	$this->knockArray["d"]=self::createTriplet(5,2,0);*/
	$handle = fopen($this->configFile, "r");
	if ($handle) {
    		while (($line = fgets($handle)) !== false) {
			$line = str_replace("\n","",$line);
			$data = explode(" ",$line);
			$this->knockArray[$data[0]]=self::createTriplet($data[1],$data[2],$data[3]);
		}

	    	fclose($handle);
	} else {
    // error opening the file.
	} 
    	}

    public function printAll() {
	print_r($this->knockArray);
    }

    private function saveKnock($cid,$knock){
	file_put_contents($this->configFile,$cid . " " . $knock["braceletId"] . " " . $knock["moduleId"] . " " . $knock["knocks"] . "\n", FILE_APPEND | LOCK_EX);

    }
    public function saveAll() {
	unlink($this->configFile);
	foreach ($this->knockArray as $key => $value)
		self::saveKnock($key,$value);
    }

    public function preInsert() {
        
    }

    public function postInsert() {
        
    }

    public function preSave() {
        
    }

    public function postSave() {
        
    }

    public function preUpdate() {
        
    }

    public function postUpdate() {
        
    }

    public function preRemove() {
        
    }

    public function postRemove() {
        
    }

    /*
     * Non obligatoire mais permet de modifier l'affichage du widget si vous en avez besoin
      public function toHtml($_version = 'dashboard') {

      }
     */

    /*     * **********************Getteur Setteur*************************** */
}

class templateCmd extends cmd {
    /*     * *************************Attributs****************************** */


    /*     * ***********************Methode static*************************** */


    /*     * *********************Methode d'instance************************* */

    /*
     * Non obligatoire permet de demander de ne pas supprimer les commandes même si elles ne sont pas dans la nouvelle configuration de l'équipement envoyé en JS
      public function dontRemoveCmd() {
      return true;
      }
     */

    public function execute($_options = array()) {
        
    }

    /*     * **********************Getteur Setteur*************************** */
}


$test = new template;
$test->load();
//$test->printAll();
$test->saveAll();
echo $test->arrayKnock;
?>
