
def renvoie_bloc_hitbox(bloc):
    return bloc[0],bloc[1],bloc[0]+bloc[2],bloc[1]+bloc[3]

def calculer_vitesse_saut(vitesse_y, en_air, force_saut, gravite, dt, vitesse_max_chute):

    if not en_air:
        return 0.0
    
    nouvelle_vitesse = vitesse_y + gravite * dt
    
    if nouvelle_vitesse > vitesse_max_chute:
        nouvelle_vitesse = vitesse_max_chute
    
    return nouvelle_vitesse

def appliquer_saut(vitesse_y, en_air, force_saut):

    if en_air:
        return vitesse_y, en_air, False 
    
    return force_saut, True, True 

def mettre_a_jour_position_y(y, vitesse_y, dt, sol_y, hauteur_joueur):

    nouvelle_y = y + vitesse_y * dt
    nouvelle_vitesse_y = vitesse_y
    nouvel_en_air = True
    if nouvelle_y + hauteur_joueur >= sol_y:
        nouvelle_y = sol_y - hauteur_joueur
        nouvelle_vitesse_y = 0.0
        nouvel_en_air = False
    
    return nouvelle_y, nouvelle_vitesse_y, nouvel_en_air

def detecter_sol(joueur_x1, joueur_y1, taillex, tailley, blocs):
    sol = float("inf")

    joueur_x2 = joueur_x1+taillex
    joueur_y2 = joueur_y1+tailley

    for bloc in blocs:
        bloc_x1, bloc_y1, bloc_x2, bloc_y2 = renvoie_bloc_hitbox(bloc)
        if bloc_x2 > joueur_x1 and bloc_x1 < joueur_x2:
            if bloc_y1 >= joueur_y2:
                if bloc_y1 < sol:
                    sol = bloc_y1
    
    return sol

def detecter_plafond(joueur_x1, joueur_y1, joueur_vitesse_y, blocs, taillex, tailley):
    joueur_x2 = joueur_x1 + taillex
    nouvelle_y1 = joueur_y1
    nouvelle_vitesse_y = joueur_vitesse_y
    if joueur_vitesse_y < 0:
        for bloc in blocs:
            bloc_x1, bloc_y1, bloc_x2, bloc_y2 = renvoie_bloc_hitbox(bloc)
            if bloc_x2 > joueur_x1 and bloc_x1 < joueur_x2:
                if joueur_y1 < bloc_y2 and joueur_y1 > bloc_y1:
                    nouvelle_vitesse_y = 0.0
                    nouvelle_y1 = bloc_y2      


    return nouvelle_y1, nouvelle_vitesse_y
                
def detecter_collisions_murs(joueur_x1, joueur_y1,joueur_vitesse_y, blocs,taillex,tailley,mur,dt):
    collision_gauche = False
    collision_droite = False

    joueur_x2 = joueur_x1+taillex
    joueur_y2 = joueur_y1 +tailley

    nouvelle_vitesse_y = joueur_vitesse_y

    for bloc in blocs:
        bloc_x1, bloc_y1, bloc_x2, bloc_y2 = renvoie_bloc_hitbox(bloc)
        if joueur_y2 > bloc_y1 and joueur_y1 < bloc_y2 :
            if joueur_x2 >= bloc_x1 and joueur_x1 < bloc_x1:
                collision_droite = True
                joueur_x1 =bloc_x1 -taillex

                if joueur_vitesse_y < 0:
                    nouvelle_vitesse_y += mur *dt

            elif joueur_x1 <= bloc_x2 and joueur_x2 > bloc_x2:
                collision_gauche = True
                joueur_x1 = bloc_x2

                if joueur_vitesse_y < 0:
                    nouvelle_vitesse_y += mur * dt

    return collision_gauche, collision_droite, nouvelle_vitesse_y , joueur_x1

def mettre_a_jour_physique_complete(x, y, vitesse_y, en_air, blocs, dt, largeur_joueur, hauteur_joueur,gravite, force_saut, vitesse_max_chute, correction_mur):
    sol_y = detecter_sol(x,y,largeur_joueur,hauteur_joueur,blocs)

    nouvelle_vitesse_y = calculer_vitesse_saut(vitesse_y, en_air, force_saut, gravite, dt, vitesse_max_chute)
    
    nouvelle_y, nouvelle_vitesse_y, nouvel_en_air = mettre_a_jour_position_y(y, nouvelle_vitesse_y, dt, sol_y, hauteur_joueur)
    
    collision_gauche, collision_droite, nouvelle_vitesse_y,x = detecter_collisions_murs(x, y ,nouvelle_vitesse_y, blocs,largeur_joueur,hauteur_joueur,correction_mur,dt)
    nouvelle_y ,nouvelle_vitesse_y = detecter_plafond(x,nouvelle_y,nouvelle_vitesse_y,blocs,largeur_joueur,hauteur_joueur)
    return x, nouvelle_y, nouvelle_vitesse_y, nouvel_en_air, collision_gauche, collision_droite

def gerer_saut(vitesse_y, en_air, force_saut,clavier=["z"],manette=["A"],joy =False):
    import GrnGame
    boolean = False
    for t1 in clavier:
        if GrnGame.clav.juste_presser(t1):
            boolean =True
    for t1 in manette:
        if GrnGame.man.juste_presse(t1):
            boolean =True
    if joy:
        joyx,joyy,joyx2,joyy2,t1,t2= GrnGame.man.renvoie_joysticks(0.5)
        if joyy <-0.5 or joyy2<-0.5:
            boolean=True
        
    if boolean:
        nouvelle_vy, nouvel_en_air, saute = appliquer_saut(vitesse_y, en_air, force_saut)
        if saute:
            return nouvelle_vy, nouvel_en_air
    return vitesse_y, en_air

def platformer_2d(dt, pos_x, pos_y, larg_joueur, haut_joueur, vitesse_y, est_en_air, blocs, 
                  gravite=400.0, force_saut=-200.0, vitesse_max_chute=500.0, correction_mur=100.0, 
                  touches_clavier_saut=["z"], touches_manette_saut=["Y"], joy_saut=False):
    
    vitesse_y, est_en_air = gerer_saut(vitesse_y, est_en_air, force_saut, 
                                        touches_clavier_saut, touches_manette_saut, joy_saut)
    
    return mettre_a_jour_physique_complete(pos_x, pos_y, vitesse_y, est_en_air, blocs, dt, 
                                           larg_joueur, haut_joueur, gravite, force_saut, 
                                           vitesse_max_chute, correction_mur)
