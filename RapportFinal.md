# PFE 1619 Knocklet
## Introduction

Ce rapport final a pour but de conclure le projet de fin d'études Knocklet. Ce projet s'est déroulé de Septembre 2016 à février 2017. Il a été réalisé par un groupe de cinq étudiants de l'ECE Paris :

* Félicien Catherin, Majeure SI
* Paul-Adrien Cordonnier, Majeure SI
* Flavien Doffagne, Majeure EE
* Nicolas Law, Chef de Projet, Majeure SE
* Pierre-Yves Napoly, Majeure SI

La domotique envahit de plus en plus nos maisons. Il est simple et peu onéreux d'obtenir des lampes et thermostat connectés afin de mieux gérer sa consommation. Néanmoins un grand nombre de ces systèmes ne sont utilisables qu'au travers d'une télécommande ou d'une application mobile. Dans le cas d'un éclairage, il est souvent plus simple de se déplacer pour appuyer sur l'interrupteur plutôt que de chercher son téléphone, le dévérouiller, lancer l'application et éteindre l'appareil.

Knocklet est un bracelet connecté localisé dans la maison et réagissant au tapotement, appelé "Knocks". À l'aide d'une combinaison de Knocks configurable, il est possible de contrôler tous les appareils de la maison. Par exemple deux knocks dans le salon allume la télévision et trois knocks dans la chambre à coucher éteint toutes les lumières de la maison.

Ce projet s'inscrit dans une valorisation partenariat avec le groupe Ingenico, leader mondial des terminaux de paiement, qui  recherchait au travers de cette réalisation obtenir un travail de recherche sur les technologies sans fil actuelles et notamment le Bluetooth Low Energy.

Ce rapport présentera tout d'abord un bref état de l'art des systèmes domotiques existant, que ce soit en termes d'appareils, d'actionneurs et de contrôleurs ainsi que les grands acteurs de ce marché. Nous rappelerons ensuite le cahier des charges puis nous décrirons notre prototype final en expliquant les choix technologiques associés à celui-ci.

## Etat de l'art et acteur du marché

TODO

## Rappel du cahier des charges

Ce projet se décompose en trois éléments séparés possédant chacun leurs spécifications.

### Le bracelet

Les fonctionnalités principales du bracelet sont :

* Le bracelet réagit au tapotement sur une surface
* Le bracelet reconnaît différents tapoments.
* Le bracelet est portable et fonctionner sur batterie rechargeable.
* Le bracelet communique avec les émetteurs/récepteurs knocklet, en permettant une localisation à l'aide de ces modules.
* Le bracelet est configurable

### Les modules émetteurs/récepteurs

Les fonctionnalités principales du bracelet sont :

* Le module reçoit des messages envoyés par les bracelets.
* Le module communique avec la centrale domotique.
* Le module fonctionne sur batterie.

### La centrale domotique

Les fonctionnalités principales de la centrale domotique sont :

* La centrale reçoit événements transférés depuis les modules. Un événement est la combinaison d'un nombre de knocks, d'un identifiant de bracelet et d'un identifiant de  module.
* La centrale gère une base de données associant les événements knocks avec une ou plusieurs actions sur le système domotique.
* La centrale fournit une interface de gestion permettant d'associer un événement avec une ou plusieurs actions sur le système domotique.
* La centrale propose d'ajouter un module et un bracelet facilement au système Knocklet.

## Réalisation technique (en parlant des technos)

Faire un joli schéma/algo/organigramme expliquant le fonctionnant

* Carte STMachin, accéléromètre
* Bluetooth Low Energy
* Module (service Linux écrit en go)
* HTTP, API Rest, JSON
* Jeedom (Plugin, scénarios etc)
* Communication avec Jeedom via l'API
* Interface de gestion du plugin

### Modules

Nos modules émetteurs/récepteurs se composent d'un Raspberry Pi 3 connecté en WiFi au réseau de la maison. Son fonctionnement réside dans un programme (ou daemon) qui lit en permanence les périphériques BLE à proximité. Une fois détecté, le processus lit les informations et les transmet à la box domotique.

C'est par l'intermédiaire du RSSI, une valeur correspondant à l'atténuation du signal reçu, qu'est localisé le bracelet. Le module recevant la plus faible atténuation est considéré comme le plus proche. En fonction de cette valeur, un délai est appliqué sur la transmission en HTTP et seul la première requête est traité par la box domotique. Le programme d'écoute utilise une bibliothèque BLE écrite en Go et dévéloppé par PayPal. Cette bibliothèque est disponible [sur Github](https://github.com/paypal/gatt). Ce programme se comporte comme un service Linux et est exécuté dès le lancement de la Raspberry.

### Communication avec Jeedom

Le protocole HTTP a été choisi pour communiquer avec la box domotique. Il est simple d'utilisation et ne requiert qu'une connexion WiFi qui relie les différents modules Knocklet du domicile. Les informations sur le Knock est transmis dans une requête POST avec les informations sous forme de JSON RPC. Exemple:

```JSON
{
	"method":"knock",
	"jsonrpc":"2.0",
	"params":{
		"apikey":"maClefDAPI",
		"braceletId":"4B:4E:4F:00:00:01",
		"moduleId":"8B:CE:4F:00:00:01",
		"rssi":-42,
		"knocks":2
	}
}

```

Dans le cas de l'ajout d'un module ou d'un bracelet, il est possible d'envoyer la méthode "init" qui permet d'ajouter un appareil dans le système Knocklet.


### Difficultés rencontrés

* Z Wave pas libre !!

Une difficulté qui a été mal envisagé réside dans l'utilisation du protocole Z-Wave.  Il était prévu que les modules se comportent comme des appareils Z-Wave. Malheureusement, ce protocole est propriétaire et la création d'un appareil demande l'acquisition d'une licence et d'un kit de développement auprès de Sigma Design. Ce protocole aurait néanmoins été très adapté, il est maillé, capable s'auto réparer et permet la transmission de faible quantité de données.

Nous avons ensuite essayé de communiquer en Radio Fréquence 433MHz. Bien que simple d'utilisation, l'acquisition du signal ne pouvait pas être fait par une Raspberry Pi. Finalement c'est HTTP qui a été choisi par soucis de simplicité.

* Lib en python relou

Une grande partie du développement du programme des modules s'est trouvé dans le choix d'une librairie adéquate. Celle-ci devait être :
* Performante
* Posséder une API simple
* Capable de gérer les broadcast BLE nativement

Une première ébauche fut réalisé en utilisant une librairie python qui s'est révélé être peu fiable sur la durée. Une librairie en Node.js a elle aussi été testé mais elle ne lisait pas les informations envoyés dans l'advertisement Bluetooth. C'est donc une librairie écrite en Go qui répondait le plus à nos critères et notamment de performances. 
