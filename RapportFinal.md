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

### Difficultés

* Z Wave pas libre !!
* Lib en python relou
* ?
