# Abstract PFE 1619

La domotique est un enjeu de plus en plus présent dans nos maisons. Il est facile et peu onéreux de se procurer des lampes connectés, de surveiller sa consommation électrique ou de chauffage à l'aide de son smartphone ou de fermer tous ses volets d'un simple appui sur une télécommande.

Néanmoins, des améliorations restent à faire, des études ont montré que 70% des appareils domotiques sont considérés comme des gadgets anxiogènes. L'ergonomie ne convient clairement pas et 50% des utilisateurs souhaitent une solution plus agréable pour piloter leur maison.

En effet, il est parfois plus simple pour allumer la lumière de se lever de son canapé et d'appuyer sur un interrupteur plutôt que de chercher son smartphone, le déverouiller, lancer l'application et appuyer sur le bon bouton. Dans le cas d'une télécommande, le problème qu'on recontre bien souvent c'est la perte de cette dernière entre deux coussins du canapé.

Actuellement il existe un grand nombre d'acteurs sur le marché de la domotique, que ce soit des périphériques (Philips Hue, Z-Wave Alliance ...), des contrôleurs (Jeedom, Somfy, Fibaro), des actionneurs vocaux (Amazon Echo, Google Home) et physiques.

Il suffit de voir la page d'accueil de Kickstarter ou les start-up présentes au CES pour découvrir de nouvelles manières d'intéragir avec les périphériques de votre maison.

Notre projet Knocklet est un bracelet connecté et localisé dans la maison connectée, il réagit aux tapotements sur une surface, appelé "Knocks". Il se connecte à une box domotique et peut donc contrôler toute lampe, thermostat, prises et même votre smartphone !

Son principe est simple, une fois porté au poignet, le bracelet détecte les coups portés sur un mur. Le module Knocklet le plus proche va ensuite récupérer l'information et la transmettre à la box domotique qui agira suivant la configuration de l'utilisateur.

Cet actionneur présente plusieurs avantages pour l'utilisateur :

* La télécommande est toujours à portée de main
* 3 coups dans la chambre à coucher n'ont pas la même utilité que dans la cuisine
* Des commandes globales à toute la maison sont possibles, comme par exemple 4 coups pour éteindre toute les lumières de ma maison.

La configuration du nombre de coups se fait au travers d'un plugin de la box domotique accessible via le web. Il est possible d'associer des scénarios déjà existants et ce pour chaque module et bracelet du système Knocklet.

La communication entre le bracelet et le module se fait à l'aide du Bluetooth LE. La valeur d'atténuation intégré au protocole permet de localiser le bracelet par rapport au module et donc dans la maison. Le module communique ensuite cette information à la box domotique en HTTP.
