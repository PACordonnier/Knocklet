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

class knocklet extends eqLogic {
    /*     * *************************Attributs****************************** */

//         * ***********************Methode static*************************** 

	public static function saveCmdConfigFromJson($js) {
		//Fonction appelée par l'API pour enregistrer les configurations depuis le plugin
		$array=json_decode($js);
		foreach($array as $key => $value)
			self::saveCmdConfig($key,$value[0],$value[1],$value[2]);
	}

	private static function createTriplet($bId,$mId,$knocks) {
        //retourne un tableau contenant les informations pour chaque "key"
        return array("braceletId"=>$bId,"moduleId"=>$mId,"knocks"=>$knocks);

    }


	public static function saveCmdConfig($cid,$bid,$mid,$knocks){
		config::save("cmd::".$cid,json_encode(self::createTriplet($bid,$mid,$knocks)),"knocklet");
	}

	public static function getTripletFromCmdId($id){
		return config::byKey("cmd::".$id,"knocklet");
	}

	public static function getCmdIdFromTriplet($bid,$mid,$knocks){
		$cmds = array();
		foreach(config::searchKey("cmd","knocklet") as $tab){
			$key = $tab["key"];
			$cmd = $tab["value"];
			if($cmd["braceletId"] == $bid && $cmd["moduleId"] == $mid && $cmd["knocks"] == $knocks) $cmds[]=filter_var($key,FILTER_SANITIZE_NUMBER_INT);
		}
		return $cmds;
	}

	public static function getScenarioIdFromTriplet($bid,$mid,$knocks){
                //TODO getScnerarioIdFromTriplet
        }


	public static function macExists($mac){
		$bool =false;
		foreach (eqLogic::byType("knocklet") as $eqLogic) {
			if($eqLogic->getConfiguration("MAC") == $mac)
				$bool = true ;
		}
		return $bool;
	}

	public static function createBracelet($name,$mac){
		if(!self::macExists($mac)){
			$eqLogic = new eqLogic();
			$eqLogic->setEqType_name('knocklet');
			$eqLogic->setName($name);
			$eqLogic->setConfiguration("type","bracelet");
			$eqLogic->setConfiguration("MAC",$mac);
			$eqLogic->save();
		}
	}

	public static function createModule($name,$mac){
		if(!self::macExists($mac)){
			$eqLogic = new eqLogic();
			$eqLogic->setEqType_name('knocklet');
			$eqLogic->setName($name);
			$eqLogic->setConfiguration("type","module");
			$eqLogic->setConfiguration("MAC",$mac);
			$eqLogic->save();
		}
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
