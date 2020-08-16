#include "naobehavior.h"
#include "../rvdraw/rvdraw.h"

extern int agentBodyType;

/*
 * Real game beaming.
 * Filling params x y angle
 */
void NaoBehavior::beam( double& beamX, double& beamY, double& beamAngle ) {

    if(worldModel->getPlayMode() == 0){

        if(worldModel->getUNum() == 1){
		    beamX = -HALF_FIELD_X /2;
            beamY = -2;
            beamAngle = 0;
        }
        else if(worldModel->getUNum() == 2){
		    beamX = -HALF_FIELD_X /2;
            beamY = 2;
            beamAngle = 0;
        }


    }
    else if(worldModel->getPlayMode() == 1){
        if(worldModel->getUNum() == 1){
		    beamX = 0;
            beamY = 0;
            beamAngle = 0;
        }
        else if(worldModel->getUNum() == 2){
		    beamX = -HALF_FIELD_X + worldModel->getUNum();
            beamY = 0;
            beamAngle = 0;
        }
    }
}

SkillType NaoBehavior::gotoFieldPosBeforeKickOff(int pNum){
    double x = -1;
	double y = -1;
	double angle = -1;
    
    if(pNum== 1){
		    x = -HALF_FIELD_X /2;
            y = -2;
            angle = 0;
        }
    else if(pNum == 2){
		    x = -HALF_FIELD_X /3;
            y = 2;
            angle = 0;
    }

    return SmartGoToTarget(VecPosition(x,y,0)); 
}

SkillType NaoBehavior::gotoFieldPosOurKickOff(int pNum){
    double x = -1;
	double y = -1;
	double angle = -1;
    
    if(pNum== 1){
		    x = 0;
            y = 0;
            angle = 0;
        }
    else if(pNum == 2){
		    x = -0.5;
            y = 3;
            angle = 0;
    }

    return SmartGoToTarget(VecPosition(x,y,0)); 
}

SkillType NaoBehavior::gotoFieldPosTheirKickOff(int pNum){
    double x = -1;
	double y = -1;
	double angle = -1;
    
    if(pNum== 1){
            WorldObject* opponent1 = worldModel->getWorldObject(WO_OPPONENT1);
            VecPosition temp = opponent1->pos;
		    x = -HALF_FIELD_X /3;
            y = temp.getY();
            angle = 0;
        }
    else if(pNum == 2){
		    WorldObject* opponent2 = worldModel->getWorldObject(WO_OPPONENT2);
            VecPosition temp = opponent2->pos;
		    x = -HALF_FIELD_X /3;
            y = temp.getY();
            angle = 0;
    }

    return SmartGoToTarget(VecPosition(x,y,0)); 
}


SkillType NaoBehavior::gotoFieldPosDefensive(){
	double x = -1;
	double y = -1;
	double angle = -1;

    if(worldModel->getUNum() == 1){
		 x = -HALF_FIELD_X ;
    	 y = 0;
    	 angle = 0;
	} 
    else if(worldModel->getUNum() == 2){
		 x = -HALF_FIELD_X +4 ;
    	 y = 2;
    	 angle = 0;
    }

    return SmartGoToTarget(VecPosition(x,y,0)); 

}

SkillType NaoBehavior::gotoFieldPosOffensive(){
	double x = -1;
	double y = -1;
	double angle = -1;

    if(worldModel->getUNum() == 1){
		 x = -HALF_FIELD_X ;
    	 y = 0;
    	 angle = 0;
	} 
    else if(worldModel->getUNum() == 2){
		 x = -HALF_FIELD_X +4 ;
    	 y = 2;
    	 angle = 0;
    }

    return SmartGoToTarget(VecPosition(x,y,0)); 

}



SkillType NaoBehavior::selectSkill() {


    worldModel->getRVSender()->clear();

    int _playerNumber = worldModel->getUNum();
    int _playMode = worldModel->getPlayMode();
    WorldObject* _gameObject = worldModel->getWorldObject(_playerNumber);
    VecPosition _myPos = worldModel->getMyPosition();

    //printf("Player : %d, X : %f, Y : %f, Z : %f\n",_playerNumber, _myPos.getX(), _myPos.getY(), _myPos.getZ());
    

    if(_playMode == PM_BEFORE_KICK_OFF){
        return gotoFieldPosBeforeKickOff(_playerNumber);
    }
    else if(_playMode == PM_KICK_OFF_LEFT){
        if(worldModel->getSide() == SIDE_LEFT){
            if(_playerNumber == 1){
                return kickBall(KICK_IK, VecPosition(-0.5, 3, 0));
            }
            else{
                return gotoFieldPosOurKickOff(_playerNumber);
            }
        }
        else{
             return gotoFieldPosTheirKickOff(_playerNumber);
        }
    }
    else if(_playMode == PM_KICK_OFF_RIGHT){
        if(worldModel->getSide() == SIDE_RIGHT){
            if(_playerNumber == 1){
                return kickBall(KICK_IK, VecPosition(-0.5, 3, 0));
            }
            else{
                return gotoFieldPosOurKickOff(_playerNumber);
            }
        }
        else{
             return gotoFieldPosTheirKickOff(_playerNumber);
        }
    }
    else if(_playMode == PM_KICK_OFF_LEFT){
        if(worldModel->getSide() == SIDE_LEFT){
            if(_playerNumber == 1){
                return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0));
            }
            else{
                return SKILL_STAND;
            }
        }
        else {
            return SKILL_STAND;
        }
    }
    else if(_playMode == PM_FREE_KICK_RIGHT){
        if(worldModel->getSide() == SIDE_RIGHT){
            if(_playerNumber == 1){
                return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0));
            }
            else{
                return SKILL_STAND;
            }
        }
        else {
            return SKILL_STAND;
        }
    }

    //return DribblingPairsSmartish();


    double* _teamMateDistances = NULL;
    double* _opponentDistances = NULL;

    _teamMateDistances = (double *) malloc(sizeof(double) * NUM_AGENTS);
    _opponentDistances = (double *) malloc(sizeof(double) * NUM_AGENTS);

    DistanceToBallArrayTeammates(_teamMateDistances); 
    DistanceToBallArrayOpponents(_opponentDistances); 

    return BasicTree(_myPos,_playerNumber,_teamMateDistances,_opponentDistances);

    //bool tr = isPossesion(_opponentDistances,_teamMateDistances);
/*     printf("====== %d : Decision Tree \n",_playerNumber);
    
    if(isPossesion(_opponentDistances,_teamMateDistances)){
        printf("====== %d : DT/Possesion \n",_playerNumber);
        if(isClosestTeam(_playerNumber,_teamMateDistances)){
            printf("====== %d : DT/Possesion/Closest \n",_playerNumber);
            if(isSafeToKick(_opponentDistances)){
                printf("====== %d : DT/Possesion/Closest/SafeKick \n",_playerNumber);
                if(isShootingRange(_myPos)){
                        printf("====== %d : DT/Possesion/Closest/SafeKick/ShootingRange \n",_playerNumber);
                    return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0));
                }
                else{
                        printf("====== %d : DT/Possesion/Closest/SafeKick/NotShootingRange \n",_playerNumber);
                    VecPosition closestTeammate = getClosestTeammatePos(_playerNumber,_teamMateDistances);
                    return kickBall(KICK_IK, VecPosition(closestTeammate.getX()+2, closestTeammate.getY(), 0));
                }
            }
            else{
                    printf("====== %d : DT/Possesion/Closest/NotSafeKick \n",_playerNumber);
                return kickBall(KICK_DRIBBLE, VecPosition(HALF_FIELD_X, 0, 0));
            }
        }
        else{
            printf("====== %d : DT/Possesion/NotClosest \n",_playerNumber);
            if(isSafeToKick(_opponentDistances)){ 
                printf("====== %d : DT/Possesion/NotClosest/SafeKick \n",_playerNumber);
                
                VecPosition target = VecPosition(_myPos.getX()+2,_myPos.getY(),0);
                if(atDestination(_myPos, target)){
                    return FaceBall(_myPos);
                }
                return goToTarget(target);
            }
            else{
                printf("====== %d : DT/Possesion/NotClosest/NotSafeKick \n",_playerNumber);
                FaceBall(_myPos);
            }
        }
    }
    else{
        printf("====== %d : DT/NotPossesion \n",_playerNumber);
        if(isClosestTeam(_playerNumber,_teamMateDistances)){
            printf("====== %d : DT/NotPossesion/Closest \n",_playerNumber);
            if(isFarToBall(_playerNumber,_teamMateDistances)){
                printf("====== %d : DT/NotPossesion/Closest/BallFar \n",_playerNumber);
                VecPosition target = VecPosition(ball.getX()-1,ball.getY(),0);
                if(atDestination(_myPos, target)){
                    return FaceBall(_myPos);
                }
                return goToTarget(target);
            }
            else{
                printf("====== %d : DT/NotPossesion/Closest/NotBallFar \n",_playerNumber);
                return goToTarget(ball);
            }

        }
        else{
            printf("====== %d : DT/NotPossesion/NotClosest \n",_playerNumber);
            if(isFarToBall(_playerNumber,_teamMateDistances)){
                printf("====== %d : DT/NotPossesion/Closest/BallFar \n",_playerNumber);
                VecPosition target = VecPosition(ball.getX()-5,ball.getY(),0);
                if(atDestination(_myPos, target)){
                    return FaceBall(_myPos);
                }
                return goToTarget(target);
            }
            else{
                printf("====== %d : DT/NotPossesion/Closest/NotBallFar \n",_playerNumber);
                VecPosition target = VecPosition(ball.getX()-3,ball.getY()+1,0);
                if(atDestination(_myPos, target)){
                    return FaceBall(_myPos);
                }
                return goToTarget(target);
            }

        }
    } */
        


    

    


    // My position and angle
    //cout << worldModel->getUNum() << ": " << worldModel->getMyPosition() << ",\t" << worldModel->getMyAngDeg() << "\n";

    // Position of the ball
    //cout << worldModel->getBall() << "\n";

    // Example usage of the roboviz drawing system and RVSender in rvdraw.cc.
    // Agents draw the position of where they think the ball is
    // Also see example in naobahevior.cc for drawing agent position and
    // orientation.
    /*
    worldModel->getRVSender()->clear(); // erases drawings from previous cycle
    worldModel->getRVSender()->drawPoint("ball", ball.getX(), ball.getY(), 10.0f, RVSender::MAGENTA);
    */

    // ### Demo Behaviors ###

    // Walk in different directions
    //return goToTargetRelative(VecPosition(1,0,0), 0); // Forward
    //return goToTargetRelative(VecPosition(-1,0,0), 0); // Backward
    //return goToTargetRelative(VecPosition(0,1,0), 0); // Left
    //return goToTargetRelative(VecPosition(0,-1,0), 0); // Right
    //return goToTargetRelative(VecPosition(1,1,0), 0); // Diagonal
    //return goToTargetRelative(VecPosition(0,1,0), 90); // Turn counter-clockwise
    //return goToTargetRelative(VecPdosition(0,-1,0), -90); // Turn clockwise
    //return goToTargetRelative(VecPosition(1,0,0), 15); // Circle

    // Walk to the ball
    //return goToTarget(ball);

    // Turn in place to face ball
    /*double distance, angle;
    getTargetDistanceAndAngle(ball, distance, angle);
    if (abs(angle) > 10) {
      return goToTargetRelative(VecPosition(), angle);
    } else {
      return SKILL_STAND;
    }*/

    // Walk to ball while always facing forward
    //return goToTargetRelative(worldModel->g2l(ball), -worldModel->getMyAngDeg());

    // Dribble ball toward opponent's goal
    //return kickBall(KICK_DRIBBLE, VecPosition(HALF_FIELD_X, 0, 0));

    // Kick ball toward opponent's goal
    //return kickBall(KICK_FORWARD, VecPosition(HALF_FIELD_X, 0, 0)); // Basic kick
    //return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0)); // IK kick

    // Just stand in place
    return SKILL_STAND;

    // Demo behavior where players form a rotating circle and kick the ball
    // back and forth
    //return demoKickingCircle();
}


/*
 * Demo behavior where players form a rotating circle and kick the ball
 * back and forth
 */
SkillType NaoBehavior::demoKickingCircle() {
    
    // Parameters for circle
    VecPosition center = VecPosition(-HALF_FIELD_X/2.0, 0, 0);
    double circleRadius = 5.0;
    double rotateRate = 2.5;

    // Find closest player to ball
    int playerClosestToBall = -1;
    double closestDistanceToBall = 10000;
    for(int i = WO_TEAMMATE1; i < WO_TEAMMATE1+NUM_AGENTS; ++i) {
        VecPosition temp;
        int playerNum = i - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum) {
            // This is us
            temp = worldModel->getMyPosition();
        } else {
            WorldObject* teammate = worldModel->getWorldObject( i );
            if (teammate->validPosition) {
                temp = teammate->pos;
            } else {
                continue;
            }
        }
        temp.setZ(0);

        double distanceToBall = temp.getDistanceTo(ball);
        if (distanceToBall < closestDistanceToBall) {
            playerClosestToBall = playerNum;
            closestDistanceToBall = distanceToBall;
        }
    }

    if (playerClosestToBall == worldModel->getUNum()) {
        // Have closest player kick the ball toward the center
        return kickBall(KICK_FORWARD, center);
    } else {
        // Move to circle position around center and face the center
        VecPosition localCenter = worldModel->g2l(center);
        SIM::AngDeg localCenterAngle = atan2Deg(localCenter.getY(), localCenter.getX());

        // Our desired target position on the circle
        // Compute target based on uniform number, rotate rate, and time
        VecPosition target = center + VecPosition(circleRadius,0,0).rotateAboutZ(360.0/(NUM_AGENTS-1)*(worldModel->getUNum()-(worldModel->getUNum() > playerClosestToBall ? 1 : 0)) + worldModel->getTime()*rotateRate);

        // Adjust target to not be too close to teammates or the ball
        target = collisionAvoidance(true /*teammate*/, false/*opponent*/, true/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);

        if (me.getDistanceTo(target) < .25 && abs(localCenterAngle) <= 10) {
            // Close enough to desired position and orientation so just stand
            return SKILL_STAND;
        } else if (me.getDistanceTo(target) < .5) {
            // Close to desired position so start turning to face center
            return goToTargetRelative(worldModel->g2l(target), localCenterAngle);
        } else {
            // Move toward target location
            return goToTarget(target);
        }
    }
}

SkillType NaoBehavior::BasicTree(VecPosition _myPos, int pNum, double* _teamMateDistances, double* _opponentDistances){
    
    //printf("PNUM : %d, X : %f, Y : %f, Z : %f\n",pNum, _myPos.getX(), _myPos.getY(), _myPos.getZ());
    
    std::string SpNum = std::to_string(pNum);

    if (pNum == 1){
        //printf("HERE\n");
        if(isClosestPlayertoBall(pNum, _teamMateDistances, _opponentDistances)){
            if(isSafeToKick(_opponentDistances)){
                if(isShootingRange(_myPos)){
                    worldModel->getRVSender()->drawCircle(SpNum,_myPos.getX(), _myPos.getY(),1,1,0,0); //RED
                    return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0));    
                }
                else{
                    VecPosition _closestTeammate = getClosestTeammatePos(pNum,_teamMateDistances);
                    worldModel->getRVSender()->drawCircle(SpNum,_myPos.getX(), _myPos.getY(),1,1,0.5,0); //ORANGE
                    return kickBall(KICK_IK, VecPosition(_closestTeammate.getX(), _closestTeammate.getY() + 5, 0));   
                }
            }
            else{
                worldModel->getRVSender()->drawCircle(SpNum,_myPos.getX(), _myPos.getY(),1,1,0,1); //PINK
                return kickBall(KICK_DRIBBLE, VecPosition(HALF_FIELD_X, 0, 0));
            }
        }
        else{
            worldModel->getRVSender()->drawCircle(SpNum,_myPos.getX(), _myPos.getY(),1,1,1,0); //YELLOW

            if(_teamMateDistances[pNum-1]<1){
                return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0));    
            }
            return SmartGoToTarget(ball);
        }
    }
    else{
        
        if(isClosestPlayertoBall(pNum, _teamMateDistances, _opponentDistances)){
            if(isSafeToKick(_opponentDistances)){
                if(isShootingRange(_myPos)){
                    worldModel->getRVSender()->drawCircle(SpNum,_myPos.getX(), _myPos.getY(),1,0,0.5,1); //BLUE
                    return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0));    
                }
                else{
                    VecPosition _closestTeammate = getClosestTeammatePos(pNum,_teamMateDistances);
                    worldModel->getRVSender()->drawCircle(SpNum,_myPos.getX(), _myPos.getY(),1,0,1,0.5); //BLUE-GREEN
                    return kickBall(KICK_IK, VecPosition(_closestTeammate.getX(), _closestTeammate.getY() + 5, 0));   
                }
            }
            else{
                worldModel->getRVSender()->drawCircle(SpNum,_myPos.getX(), _myPos.getY(),1,0,1,1); //CYAN
                return kickBall(KICK_DRIBBLE, VecPosition(HALF_FIELD_X, 0, 0));
            }
        }
        else{
            if(worldModel->distanceToMyGoal(ball) < 15){
                VecPosition target = VecPosition(ball.getX()-5,ball.getY(),0);
                target = ValidateFieldPos(target);
                worldModel->getRVSender()->drawCircle(SpNum,_myPos.getX(), _myPos.getY(),0,0,0,0); // WHITE
                return SmartGoToTarget(target);
            }
            else{
                VecPosition target = VecPosition(ball.getX()+5,ball.getY()-2,0);
                printf("Pnum = %d,Target x=%f,y=%f,z=%f",pNum,target.getX(),target.getY(),target.getZ());

                target = ValidateFieldPos(target);
                printf("PnumAdjusted = %d,Target x=%f,y=%f,z=%f",pNum,target.getX(),target.getY(),target.getZ());
                worldModel->getRVSender()->drawCircle(SpNum,_myPos.getX(), _myPos.getY(),1,0.5,0,1); //VIOLET
                return SmartGoToTarget(target);
            }

            /*
            if(isPossesion(_opponentDistances, _teamMateDistances)){
                VecPosition target = VecPosition(ball.getX()+5,ball.getY()-2,0);
                target = ValidateFieldPos(target);
                worldModel->getRVSender()->drawCircle(SpNum,_myPos.getX(), _myPos.getY(),1,0.5,0,1); //VIOLET
                return SmartGoToTarget(target);
            }
            else{
                VecPosition target = VecPosition(ball.getX()-5,ball.getY(),0);
                target = ValidateFieldPos(target);
                worldModel->getRVSender()->drawCircle(SpNum,_myPos.getX(), _myPos.getY(),0,0,0,0); // WHITE
                return SmartGoToTarget(target);
            }
            */
        }
        

    }
    
    worldModel->getRVSender()->drawCircle(SpNum,_myPos.getX(), _myPos.getY(),0,1,1,1); // BLACK
    return SKILL_STAND;

}

void NaoBehavior::DistanceToBallArrayTeammates(double* _teamMateDistances){


    for(int i = WO_TEAMMATE1; i < WO_TEAMMATE1+NUM_AGENTS; ++i) {//Our Players

        WorldObject* teammate = worldModel->getWorldObject(i);
        VecPosition temp;
        if (teammate->validPosition) {
                temp = teammate->pos;
            } else {
                continue;
            }
        temp.setZ(0);
        float distance = temp.getDistanceTo(ball);
        if(distance >= 0.1){
             _teamMateDistances[i-WO_TEAMMATE1] = distance;
        }
        else{
            _teamMateDistances[i-WO_TEAMMATE1] = MAXFLOAT;
        }

       
    }       
    /*
	for(int z=0;z<NUM_AGENTS;++z){ 
        printf("T_ID = %d : %f \n",z+1,_teamMateDistances[z]);
	}
    */
}

void NaoBehavior::DistanceToBallArrayOpponents(double* _opponentDistances){

    //printf("NumA = %d \n",NUM_AGENTS);
    for(int i = WO_OPPONENT1; i < WO_OPPONENT1+NUM_AGENTS; ++i) {//Their Players
        WorldObject* opponent = worldModel->getWorldObject(i);
        VecPosition temp;
        if (opponent->validPosition) {
                temp = opponent->pos;
            } else {
                continue;
            }
        temp.setZ(0);
        float distance = temp.getDistanceTo(ball);
        if(distance >= 0.1){
             _opponentDistances[i-WO_OPPONENT1] = distance;
        }
        else{
            _opponentDistances[i-WO_OPPONENT1] = MAXFLOAT;
        }

    }    
    /*
	for(int z=0;z<NUM_AGENTS;++z){ 
        printf("O_ID = %d : %f \n",z+1,_opponentDistances[z]);
	}
    */
}

bool NaoBehavior::isClosestPlayertoBall(int pNum, double* _opponentDistances, double* _teamMateDistances){
    int arrlengthOpp = NUM_AGENTS;
    int arrlengthTeam = NUM_AGENTS;

    double minOppDis = MAXFLOAT; 
    for(int z=0;z<arrlengthOpp;++z){

        printf("pNum : %d oppnum : %d distance : %f\t",pNum,z,_opponentDistances[z]);
        if(_opponentDistances[z]< minOppDis){
            minOppDis = _opponentDistances[z];
        } 
	}
    printf("\n");
    double minTeamDis = MAXFLOAT; 
    for(int z=0;z<arrlengthTeam;++z){
        printf("pNum : %d teamnum : %d distance : %f\t",pNum,z,_teamMateDistances[z]);
        if(_teamMateDistances[z]< minTeamDis){
            minTeamDis = _teamMateDistances[z];
        } 
	}
    printf("\n");

    printf("minOpp = %f, minTeam = %f\n", minOppDis+1,minTeamDis);
    if(minOppDis+1 < minTeamDis){
        return false;
    }
    else{
        printf("_teamMateDistances = %f, minTeam = %f\n", _teamMateDistances[pNum-1],minTeamDis);
        if(minTeamDis != _teamMateDistances[pNum-1]){
            return false;
        }
        else{
            return true;
        }
    }
}

bool NaoBehavior::isPossesion(double* _opponentDistances, double* _teamMateDistances){


    int arrlengthOpp = NUM_AGENTS;
    int arrlengthTeam = NUM_AGENTS;

    double minOppDis = MAXFLOAT; 
    for(int z=0;z<arrlengthOpp;++z){
        if(_opponentDistances[z]< minOppDis){
            minOppDis = _opponentDistances[z];
        } 
	}
    double minTeamDis = MAXFLOAT; 
    for(int z=0;z<arrlengthTeam;++z){
        if(_teamMateDistances[z]< minTeamDis){
            minTeamDis = _teamMateDistances[z];
        } 
	}
    if(minOppDis < minTeamDis+1){
        return false;
    }
    return true;
}

bool NaoBehavior::isClosestTeam(int _playerNumber, double* _teamMateDistances){
    int arrlengthTeam = NUM_AGENTS;

    //printf("Size = %d",arrlengthTeam);
    double minTeamDis = MAXFLOAT; 
    int id = -1;
    for(int z=0;z<arrlengthTeam;++z){
        if((_teamMateDistances[z]+1)< minTeamDis){
            minTeamDis = _teamMateDistances[z];
            id = z+1;
        } 
	}
    if(_playerNumber== id){
        return true;
    }
    return false;

}

bool NaoBehavior::isSafeToKick(double* _opponentDistances){
    double minOppDis = MAXFLOAT; 
    int arrlengthOpp = NUM_AGENTS;
    for(int z=0;z<arrlengthOpp;++z){
        if(_opponentDistances[z]< minOppDis){
            minOppDis = _opponentDistances[z];
        } 
	}
    if(minOppDis <1){
        return false;
    }
    return true;

}

bool NaoBehavior::isShootingRange(VecPosition _player){
    
    double distanceToGoal = worldModel->distanceToOppGoal(_player);

    if(distanceToGoal < 5){
        return true;
    }
    return false;

}

VecPosition NaoBehavior::getClosestTeammatePos(int _playerNumber,double* _teamMateDistances){
    int arrlengthTeam = NUM_AGENTS;

    //printf("Size = %d",arrlengthTeam);
    double minTeamDis = MAXFLOAT; 
    int id = -1;
    for(int z=0;z<arrlengthTeam;++z){
        if(_teamMateDistances[z]< minTeamDis){
            if(_playerNumber != z+1){
                minTeamDis = _teamMateDistances[z];
                id = z+1;
            }
        } 
	}

    return worldModel->getWorldObject(id)->pos;
    

}

VecPosition NaoBehavior::ValidateFieldPos(VecPosition target){
    if(target.getX() <= (-1*FIELD_X)/2){
        target.setX(2); 
    }
    else if(target.getX() >= FIELD_X/2 -1){
        target.setX(FIELD_X -2); 
    }

    if(target.getY() <= (-1*FIELD_Y)/2){
        target.setY(2);
    }
    else if(target.getY() >= FIELD_Y/2 -1){
        target.setY(FIELD_Y -2);
    }

    return target;
}

bool NaoBehavior::isFarToBall(int _playerNumber,double* _teamMateDistances){

    double distance = _teamMateDistances[_playerNumber-1];

    if(distance > 5){
        return true;
    }
    return false;

}

SkillType NaoBehavior::SmartGoToTarget(VecPosition target){
    SIM::AngDeg localAngle = atan2Deg(target.getY(), target.getX());

    // Adjust target to not be too close to teammates or the ball
        target = collisionAvoidance(true /*teammate*/, true/*opponent*/, false/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);

        if (me.getDistanceTo(target) < .25 && abs(localAngle) <= 10) {
            // Close enough to desired position and orientation so just stand
            return SKILL_STAND;
        } else if (me.getDistanceTo(target) < .5) {
            // Close to desired position so start turning to face center
            return goToTargetRelative(worldModel->g2l(target), localAngle);
        } else {
            // Move toward target location
            return goToTarget(target);
        }
}

SkillType NaoBehavior::DribblingPairsSmartish() {
	int myNum = worldModel->getUNum();
	
	// Determine which two players are closest to the ball
    int playerClosestToBall = -1;
	int playerSecondClosestToBall = -1;
    double closestDistanceToBall = 10000;
	double SecondclosestDistanceToBall = 10000;
    for(int i = WO_TEAMMATE1; i < WO_TEAMMATE1+NUM_AGENTS; ++i) {
        VecPosition temp;
        int playerNum = i - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum) {
            // This is us
            temp = worldModel->getMyPosition();
        } else {
            WorldObject* teammate = worldModel->getWorldObject( i );
            if (teammate->validPosition) {
                temp = teammate->pos;
            } else {
                continue;
            }
        }
        temp.setZ(0);

        double distanceToBall = temp.getDistanceTo(ball);
		if(distanceToBall < SecondclosestDistanceToBall){
		    if (distanceToBall < closestDistanceToBall) {
				playerSecondClosestToBall = playerClosestToBall;
		        playerClosestToBall = playerNum;

				SecondclosestDistanceToBall = closestDistanceToBall;	
		        closestDistanceToBall = distanceToBall;
		    }
			else{
				SecondclosestDistanceToBall = distanceToBall;
				playerSecondClosestToBall = playerNum;	
			}
		}
    }

    cout << "playerClosestToBall" << playerClosestToBall << endl;
    cout << "playerSecondClosestToBall" << playerSecondClosestToBall << endl;

	// Get the positions of the two closest players
	VecPosition SecondClosestPos = worldModel->getTeammate(playerSecondClosestToBall);
	VecPosition ClosestPos = worldModel->getTeammate(playerClosestToBall);


	VecPosition OppLeftGoalPost = worldModel->getOppLeftGoalPost();
	VecPosition OppRightGoalPost = worldModel->getOppRightGoalPost();

	VecPosition MyPos = worldModel->getMyPosition();
	VecPosition MyLast = worldModel->getMyLastPosition();
	VecPosition PosBetweenClosestAndSecond  = (ClosestPos + SecondClosestPos)/2;
	VecPosition PosBetweenSecondClosestAndTheirGoalPosition  = (OppRightGoalPost + SecondClosestPos)/2;

	cout << "MyPos" << MyPos<< endl;
	cout << "SecondClosestPos" << SecondClosestPos << endl;
	cout << "PosBetweenClosestAndSecond" << PosBetweenClosestAndSecond << endl;

	double distanceBallToGoal = worldModel->distanceToOppGoal(ball);
	
		 
	
	
	cout << "distanceBallToGoal" << distanceBallToGoal << endl;
	if(distanceBallToGoal > 5){
		if (playerClosestToBall == myNum) {
			return kickBall(KICK_FORWARD, PosBetweenSecondClosestAndTheirGoalPosition);
		}
		else{
			if(myNum==1){
				double distanceBetweenMeAndClosestPlayer = MyPos.getDistanceTo(ClosestPos);
				if(distanceBetweenMeAndClosestPlayer < 5){
					if(worldModel->distanceToOppGoal(MyPos) > 3){
						return goToTarget(OppLeftGoalPost);
					}
					else{
						return SKILL_STAND;
					}
				}
				else{
					return SKILL_STAND;
				}
			}
			else{
				double distanceBetweenMeAndClosestPlayer = MyPos.getDistanceTo(ClosestPos);
				if(distanceBetweenMeAndClosestPlayer < 5){
					if(worldModel->distanceToOppGoal(MyPos) > 3){
						return goToTarget(OppRightGoalPost);
					}
					else{
						return SKILL_STAND;
					}
				}
				else{
					return SKILL_STAND;
				}
			}
		}
	}
	else{
		if (playerClosestToBall == myNum) {
			return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0));
		}
		else{
			return goToTargetRelative(VecPosition(1,0,0), 15); // Circle
		}
	}
}


bool NaoBehavior::atDestination(VecPosition mypos, VecPosition target){
    double distance =  mypos.getDistanceTo(target);

    if(distance <1){
        return true;
    }
    return false;
}

SkillType NaoBehavior::FaceBall(VecPosition mypos){
    double distance, angle;
    getTargetDistanceAndAngle(ball, distance, angle);
    if (abs(angle) > 10) {
      return goToTargetRelative(mypos, angle);
    } else {
      return SKILL_STAND;
    }
}