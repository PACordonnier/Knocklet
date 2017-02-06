# Abstract PFE 1619

La domotique est un enjeu de plus en plus présent dans nos maisons. Il est facile et peu onéreux de se procurer des lampes connectées, de surveiller sa consommation électrique ou de chauffage à l'aide de son smartphone ou de fermer tous ses volets d'un simple appui sur une télécommande.

Néanmoins, des améliorations restent à faire, des études ont montré que 70% des appareils domotiques sont considérés comme des gadgets anxiogènes. L'ergonomie ne convient clairement pas et 50% des utilisateurs souhaitent une solution plus agréable pour piloter leur maison.

En effet, il est parfois plus simple pour allumer la lumière de se lever de son canapé et d'appuyer sur un interrupteur plutôt que de chercher son smartphone, le déverouiller, lancer l'application et appuyer sur le bon bouton. Dans le cas d'une télécommande, le problème que l'on rencontre bien souvent c'est la perte de cette dernière entre deux coussins du canapé.

Actuellement il existe un grand nombre d'acteurs sur le marché de la domotique. Que ce soit des fabricants de  périphériques (Philips Hue, Z-Wave Alliance ...), de contrôleurs (Jeedom, Somfy, Fibaro), d' actionneurs vocaux (Amazon Echo, Google Home) ou physiques.

Il suffit de voir la page d'accueil de Kickstarter ou les start-up présentes au CES pour découvrir de nouvelles manières d'intéragir avec les périphériques de son domicile.

Notre projet Knocklet est un bracelet connecté et localisé dans la maison intelligente. Il réagit aux tapotements sur une surface, appelé "Knocks". Il se connecte à une centrale domotique et peut donc contrôler toute lampe, thermostat, prises et même votre smartphone !

Son principe est simple. Une fois porté au poignet, le bracelet détecte les coups portés sur un mur. Le module de localisation le plus proche va ensuite récupérer l'information et la transmettre à la centrale domotique qui agira suivant la configuration de l'utilisateur.

Cet actionneur présente plusieurs avantages pour l'utilisateur :

* Plus de télécommandes à manipuler pour contrôler les appareils connectés.
* Les actions peuvent changer d'une pièce à l'autre.
* Les actions de chaque bracelet est personnalisable. Toute personne possédant un bracelet peut avoir ses propres commandes.
* Des commandes globales à tout le domicile sont possibles, comme par exemple éteindre toutes les lumières depuis n'importe où dans la maison.
La configuration du nombre de coups se fait au travers d'un plugin de la centrale domotique accessible via un navigateur internet. Il est possible d'associer des scénarios déjà existants et ce pour chaque module et bracelet du système Knocklet.

La communication entre le bracelet et le module se fait à l'aide du Bluetooth. La valeur de puissance de signal intégré au protocole permet de localiser le bracelet par rapport au module et donc dans la pièce dans laquelle il se trouve dans la maison. Le bracelet est équipé de la technologie Bluetooth Low Energy permettant une consommation très faible et une durée de vie plus élevé sur la batterie. Le module communique ensuite cette information à la centrale domotique en utilisant le WiFi.
