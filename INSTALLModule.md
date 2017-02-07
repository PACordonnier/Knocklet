Il est nécessaire d'installer les outils golang et bluez pour utiliser le module

Copier le contenu de go/src (contient la librairie modifiée paypal/gatt) et go/knocklet.go dans le workspace go
```BASH
go build knocklet.go
```

Bluez est installé par défaut dans la distribution raspbian jessie.

Pour lancer le programme:
```BASH
sudo ./knocklet [Prefixe bluetooth de filtre]
```

Dans le cas de l'installation en service, le script d'initialisation est fourni (knockletInit), 
il faut juste placer le binaire knocklet dans le PATH
