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

if (isset($argv)) {
	foreach ($argv as $arg) {
		$argList = explode('=', $arg);
		if (isset($argList[0]) && isset($argList[1])) {
			$_REQUEST[$argList[0]] = $argList[1];
		}
	}
}
{
	try {
		$IP = getClientIp();
		$request = init('request');
		if ($request == '') {
			$request = file_get_contents("php://input");
		}
		log::add('api', 'info', $request . ' - IP :' . $IP);

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
		if ($jsonrpc->getMethod() == 'knock') {
			if((isset($params['braceletId'])) && (isset($params['moduleId'])) && (isset($params['knocks']))){
				$now = date("Y-m-d H:i:s");
				$text = $now."\n"."[braceletId]= ".$params['braceletId']."\n[moduleId]= ".$params['moduleId']."\n[knocks]= ".$params['knocks']."\n\n";
				file_put_contents("/tmp/knockletAPI", $text, FILE_APPEND);
				
			//TODO enlever le new, passer les fonctions en static
				$knocklet= new knocklet();
				$cid = $knocklet->getIdFromTriplet($params['braceletId'],$params['moduleId'],$params['knocks']);
				if($cid == false)
					throw new Exception('Combinaison non liée à une commande', -32602);
				else{
					$cmd = cmd::byId($cid);
					$cmd->execCmd($_REQUEST);
					$jsonrpc->makeSuccess("OK !");
				}
			}

			else  throw new Exception('Missings method parameter(s) (braceletId, moduleId, knocks)', -32602);

		}

	throw new Exception('Aucune méthode correspondante : ' . secureXSS($jsonrpc->getMethod()), -32500);
/*         * *********Catch exeption*************** */
	} catch (Exception $e) {
	$message = $e->getMessage();
	$jsonrpc = new jsonrpc(init('request'));
	$errorCode = (is_numeric($e->getCode())) ? -32000 - $e->getCode() : -32599;
	$jsonrpc->makeError($errorCode, $message);
	}
}
?>
