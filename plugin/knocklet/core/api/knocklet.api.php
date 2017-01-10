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

		/*             * ************************config*************************** */
/*		if ($jsonrpc->getMethod() == 'config::byKey') {
			$jsonrpc->makeSuccess(config::byKey($params['key'], $params['plugin'], $params['default']));
		}

		if ($jsonrpc->getMethod() == 'config::save') {
			$jsonrpc->makeSuccess(config::save($params['key'], $params['value'], $params['plugin']));
		}

		if (isset($params['plugin']) && $params['plugin'] != '' && $params['plugin'] != 'core') {
			log::add('api', 'info', 'Demande pour le plugin : ' . secureXSS($params['plugin']));
			include_file('core', $params['plugin'], 'api', $params['plugin']);
		} else */ {
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

			/*             * ************************Commande*************************** */
			if ($jsonrpc->getMethod() == 'cmd::all') {
				$return = array();
				foreach (cmd::all() as $cmd) {
					$return[] = $cmd->exportApi();
				}
				$jsonrpc->makeSuccess($return);
			}


			/*             * ************************************************************************ */
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
