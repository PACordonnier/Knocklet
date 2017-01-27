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

require_once dirname(__FILE__) . '/../../../../core/php/core.inc.php';


function getLastKnock($bid){
	return file_get_contents("/tmp/".$bid);
}

function setLastKnock($bid){
	$time = microtime(true);
	file_put_contents("/tmp/".$bid,$time);
}

if (isset($argv)) {
	foreach ($argv as $arg) {
		$argList = explode('=', $arg);
		if (isset($argList[0]) && isset($argList[1])) {
			$_REQUEST[$argList[0]] = $argList[1];
		}
	}
}

try {
	$IP = getClientIp();
	$request = init('request');
	if ($request == '') {
		$request = file_get_contents("php://input");
	}
	log::add('apiKnocklet', 'info', $request . ' - IP :' . $IP);
	$jsonrpc = new jsonrpc($request);

	if ($jsonrpc->getJsonrpc() != '2.0') {
		throw new Exception('Requête invalide. Version Jsonrpc invalide : ' . $jsonrpc->getJsonrpc(), -32001);
	}
	$params = $jsonrpc->getParams();

	if ($jsonrpc->getMethod() == 'ping') {
		$jsonrpc->makeSuccess('pong');
	}

	if (!isset($params['apikey']) && !isset($params['api'])) {
		throw new Exception(__('Aucune clef API', __FILE__), -32001);
	}

	if ((isset($params['apikey']) && !jeedom::apiAccess($params['apikey'])) || (isset($params['api']) && !jeedom::apiAccess($params['api']))) {
		throw new Exception(__('Clé API invalide', __FILE__), -32001);
	}

	/*             * ***********************Version********************************* */
	if ($jsonrpc->getMethod() == 'version') {
		$jsonrpc->makeSuccess(jeedom::version());
	}

		/*             * ***********************isOk********************************* */
	if ($jsonrpc->getMethod() == 'jeedom::isOk') {
		$jsonrpc->makeSuccess(jeedom::isOK());
	}

	/*             * ***********************Datetime********************************* */
	if ($jsonrpc->getMethod() == 'datetime') {
		$jsonrpc->makeSuccess(getmicrotime());
	}



		/*             * ***********************Knocklet********************************* */
	//Logs
	if((isset($params['braceletId'])) && (isset($params['moduleId'])) && (isset($params['knocks'])) && (isset($params['rssi'])))
	file_put_contents("/tmp/postLog","timestamp => " .microtime(true)." bId => ".$params['braceletId']." mId => ".$params['moduleId']." knocks => ".$params['knocks']." rssi => ".$params['rssi']."\n", FILE_APPEND);


	if ($jsonrpc->getMethod() == 'init') {
		//Lorsque la méthode init est demandée, on ajoute les equipements détéctés dans la base de donnée (sauf si ils y sont déjà)
		if((isset($params['braceletId'])) && (isset($params['moduleId']))){
			file_put_contents("/tmp/postLog","timestamp => " .microtime(true)." bId => ".$params['braceletId']." mId => ".$params['moduleId']."\n", FILE_APPEND);
			knocklet::createBracelet("Nouveau Bracelet",$params['braceletId']);
                        knocklet::createModule("Nouveau Module",$params['moduleId']);
                        $jsonrpc->makeSuccess("Demande d'initialisation ...");
		}
		else  throw new Exception('Missings method parameter(s) (braceletId, moduleId)', -32602);

	}

	//Gestions des Knocks (conversion du triplet recu en une liste de commandes/scenarios à lancer)
	if ($jsonrpc->getMethod() == 'knock') {
		if((isset($params['braceletId'])) && (isset($params['moduleId'])) && (isset($params['knocks'])) && (isset($params['rssi']))){
			//Si l'api a reçu une commande du même bracelet moins de x secondes avant, la commande est ignorée
			if((microtime(true) - getLastKnock($params['braceletId'])) < 1.5)
				throw new Exception("Demande ignoree, derniere commande pour ce bracelet recue il y a " . (microtime(true) - getLastKnock($params['braceletId'])) . " seconde", -32604);
			//Enregistrement du timestamp du knock
			setLastKnock($params['braceletId']);
			//Récupération des ID de commandes et de scénarios
			$cids = knocklet::getCmdIdFromTriplet($params['braceletId'],$params['moduleId'],$params['knocks']);
			$sids = knocklet::getScioIdFromTriplet($params['braceletId'],$params['moduleId'],$params['knocks']);

			if(count($cids) == 0 && count($sids) == 0)
				throw new Exception('La combinaison ne correspond à aucune commande', -32605);
			else{
				foreach($cids as $cid){
					//Execute chaque commandes demandées
					$cmd = cmd::byId($cid);
					$cmd->execCmd($_REQUEST);
				}

				foreach($sids as $sid){
					//Execute chaque scenarios demandés
					$scio = scenario::byId($sid);
					$scio->execute();
				}
				$jsonrpc->makeSuccess("OK ! " . count($cids) . " commande(s) et  " . count($sids) . " scenario(s) executes");
			}

		}else  throw new Exception('Missings method parameter(s) (braceletId, moduleId, knocks, rssi)', -32602);
	}


	throw new Exception('Aucune méthode correspondante : ' . secureXSS($jsonrpc->getMethod()), -32500);
/*         * *********Catch exeption*************** */
} catch (Exception $e) {
	$message = $e->getMessage();
	$jsonrpc = new jsonrpc(init('request'));
	$errorCode = (is_numeric($e->getCode())) ? -32000 - $e->getCode() : -32599;
	$jsonrpc->makeError($errorCode, $message);
}

?>
