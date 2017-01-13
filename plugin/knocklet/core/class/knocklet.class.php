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

class knocklet extends eqLogic {
    /*     * *************************Attributs****************************** */
	private $knockArray=array();
	private $configFile="/usr/share/nginx/www/jeedom/plugins/knocklet/data/config";

    /*     * ***********************Methode static*************************** */

	public static function saveConfigFromJson($js) {
		//Fonction appelée par l'API pour enregistrer les configurations depuis le plugin
		$array=json_decode($js);
		$knock = new knocklet(1);
		foreach($array as $key => $value){
			$knock->add($key,$value[0],$value[1],$value[2]);
		}
		$knock->saveAll();
	}


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

    function __construct($a){
	//Constructeur de la classe
	//Si appelée sans paramètre, les configurations sont chargées depuis le fichier
	if($a!=1) self::load();
    }


    private function createTriplet($bId,$mId,$knocks) {
	//retourne un tableau contenant les informations pour chaque "key"
	return array("braceletId"=>$bId,"moduleId"=>$mId,"knocks"=>$knocks);

    }


    public function add($cid,$bid,$mid,$knocks) {
	//Ajouter un élément au tableau
	$this->knockArray[$cid]=self::createTriplet($bid,$mid,$knocks);

    }

    public function load() {
	//Charge les informations depuis le fichier de configuration
	$handle = fopen($this->configFile, "r");
	if ($handle) {
    		while (($line = fgets($handle)) !== false) {
			$line = str_replace("\n","",$line);
			$data = explode(" ",$line);
			if($data[3]!=0)
				$this->knockArray[$data[0]]=self::createTriplet($data[1],$data[2],$data[3]);
		}

	    	fclose($handle);
	} else {
	    // error opening the file.
	}
    }


    public function getTripletFromId($cid) {
	//Retourne le triplet correspondant à l'ID envoyé
	//Retourne false si ce triplet n'existe pas
        if(array_key_exists($cid,$this->knockArray))
                return $this->knockArray[$cid];
        else return false;
    }

    public function getIdFromTriplet($bid,$mid,$knocks) {
	//retourne l'ID correspondant au triplet envoyé
	//retourne faux si la commande recherchée n'exsite pas 
        foreach ($this->knockArray as $key => $value){
			if(self::createTriplet($bid,$mid,$knocks) == $this->knockArray[$key])
				return $key;
		}
	return false;
    }

    public function printAll() {
	//affiche toutes les configurations chargées en mémoire
	print_r($this->knockArray);
    }

    private function saveKnock($cid,$knock){
	//Ecrit la configuration d'une commande dans le tableau
	file_put_contents($this->configFile,$cid . " " . $knock["braceletId"] . " " . $knock["moduleId"] . " " . $knock["knocks"] . "\n", FILE_APPEND | LOCK_EX);

    }
    public function saveAll() {
	//Ecrit toutes les confirurations dans le fichier de config
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

class knockletCmd extends cmd {
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

?>
