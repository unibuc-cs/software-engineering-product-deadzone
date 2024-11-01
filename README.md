# Proiect-Inginerie-Software-IS

&emsp; Proiect Inginerie Software (IS) Anul 3, Semestrul 1, Facultatea de Matematica si Informatica, Universitatea din Bucuresti <br/>

### &emsp; Membrii Echipa: <br/>
Capatina Razvan Nicolae ($352$) <br/> 
Mihalache Sebastian Stefan ($352$) <br/>
Buca Mihnea Vicentiu ($352$) <br/>
Luculescu Teodor ($351$) <br/>
Petrovici Ricardo ($351$) <br/>
Ciobanu Dragos ($351$) <br/>

## Product Vision
&emsp; Proiectul este un joc 2D, un top-down shooter conceput pentru un public larg, in special pentru cei pasionati de sfera gaming-ului. Aplicatia are 2 moduri de joc: zombie survival si player vs. player. Spre deosebire de alte jocuri, aplicatia noastra are suport pentru multiplayer, permitand conectarea mai multor jucatori la aceeasi runda, ducand la o experienta colaborativa si, respectiv, competitiva. Suport-ul de multiplayer este restrans la un LAN, dar prin intermediul aplicatiei Hamachi, care creeaza un VPN, experienta poate fi extinsa si la participanti din LAN-uri diferite. Jocul este conceput astfel incat sa fie usor de personalizat de catre oricine. Harta unde au loc rundele, skin-urile jucatorilor, armelor, dar si a zombilor sunt independente de implementarea jocului si pot fi usor modificate.

### Links
- [Documentatie](https://github.com/unibuc-cs/software-engineering-product-deadzone/wiki/Documentatie)
  - Requirements
  - Features
  - User Stories
  - Diagrams

<br/>

- [Backlog](https://github.com/orgs/unibuc-cs/projects/29)

<br/> <br/>

## Setup  
1. Clone the repository
```sh
git clone --recursive https://github.com/unibuc-cs/software-engineering-product-deadzone
```

2. Change the current working directory
```sh
cd software-engineering-product-deadzone
```

3. Generate the project
```sh
generate_project.bat
```
&emsp; or <br/>
```sh
./generate_project.bat
```

4. Compile and run the generated solution
```sh
DeadZone.sln
```

## Requirements
- Visual Studio including the *"Desktop development with C++"* workload

## Adding new files to the project
Add new files directly to the 'DeadZone' folder and run `generate_project.bat`
