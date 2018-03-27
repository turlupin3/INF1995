	demarrage();
	Memoire24CXXX memoire = Memoire24CXXX();
	initialisationUART();
	unsigned char etatBouton = 0;
	unsigned char capteurGauche = 0;
	unsigned char capteurDroite = 0;
	memoire.ecriture(0, 0xf0);//nom
	memoire.ecriture(1,"J Rom Bot",13);
	memoire.ecriture(14, 0xf1);//numero dequipe
	memoire.ecriture(15,6165,1);
	memoire.ecriture(16, 0xf2);//numero de section
	memoire.ecriture(17,3,1);
	memoire.ecriture(18, 0xf3);//session
	memoire.ecriture(19,"18-1",4);
	memoire.ecriture(23, 0xf4);//numero courleur de base
	memoire.ecriture(24,1,1);
	memoire.ecriture(25, 0xf5);//VAR etat du bouton
	memoire.ecriture(26,etatBouton,1);
	memoire.ecriture(27, 0xf6);//VAR Distance en cm capteur gauche
	memoire.ecriture(28,capteurGauche,1);
	memoire.ecriture(29 0xf7);//VAR Distance en cm capteur droite
	memoire.ecriture(30,capteurDroite,1);
	
	struct Instruction {
		unsigned char type;
		unsigned char donnee;
		 };
	Instruction instruc;	 
		 
	instruc.type = receptionUART() >> 4;
	instruc.donnee = receptionUART();
	
	switch (instruc.type) {
		case 0xf8: //vitesse de roue gauche
			avancerMoteurD(instruc.donnee);
			break;
		case 0xf9: //vitesse de rouge droite
			avancerMoteurG(instruc.donnee);
			break;
		case 0xfa: // couleur de la del libre
			delSwitcher(instruc.donnee);
			break;
		case 0xfb: // requete denvoi des information didentification
		
			break;
		default:
		//error
		
		
	}



