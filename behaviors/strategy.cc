#include "naobehavior.h"
#include "../rvdraw/rvdraw.h"

extern int agentBodyType;

/*
 * Real game beaming.
 * Filling params x y angle
 */
void NaoBehavior::beam( double& beamX, double& beamY, double& beamAngle ) {
    if(worldModel->getUNum() == 1){
        beamX = -HALF_FIELD_X + 12;
    } else {
        beamX = -HALF_FIELD_X+1;
    }
    beamY = 0;
    beamAngle = 0;
}


SkillType NaoBehavior::selectSkill() {

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

    if(!improperPlayMode(worldModel->getPlayMode()) && !kickPlayMode(worldModel->getPlayMode(), false)){
        if(playerClosestToBall == worldModel->getUNum()){
            return Striker();
        } else {
            return Defender();
        }
    } else if (kickPlayMode(worldModel->getPlayMode(), false)) {
        if(playerClosestToBall == worldModel->getUNum()){
            VecPosition target = VecPosition(HALF_FIELD_X, 0, 0);
            return kickBall(KICK_IK,target);
        } else {
            return SKILL_STAND;
        }
        
    } else {
        if(playerClosestToBall == worldModel->getUNum()){
            return SKILL_STAND;
        } else {
            return Defender();
        }
    }
}

SkillType NaoBehavior::Striker() {

    worldModel->getRVSender()->drawPoint(HALF_FIELD_X, 0, 0);
    worldModel->getRVSender()->drawPoint(HALF_FIELD_X, ball.getY(), 0);
    worldModel->getRVSender()->drawPoint(ball.getX()-0.2, ball.getY(), atan(abs(ball.getY()/(ball.getX()+HALF_FIELD_X))));

    // cout << "Distance to ball " << worldModel->getOppRightGoalPost().getDistanceTo(ball) << endl;
    if(ball.getX() > worldModel->getMyPosition().getX() && worldModel->getMyPosition().getDistanceTo(ball) < 1 ){
        if(worldModel->getOppRightGoalPost().getDistanceTo(ball) < 4){
            return kickBall(KICK_IK,VecPosition(HALF_FIELD_X, 0, 0));
        } else if( worldModel->getOppRightGoalPost().getDistanceTo(ball) > 12 ) {
            VecPosition target = VecPosition(HALF_FIELD_X/2, HALF_FIELD_Y-2, 0);
            return kickBall(KICK_DRIBBLE,target);
        } else {
            VecPosition target = VecPosition(HALF_FIELD_X, 0, 0);
            return kickBall(KICK_DRIBBLE,target);
        }

    } else {
        VecPosition target = VecPosition( ball.getX()-0.2, ball.getY(), atan(ball.getY()/(ball.getX()+HALF_FIELD_X)));
        target = collisionAvoidance(true /*teammate*/, false/*opponent*/, true/*ball*/, 0.5/*proximity thresh*/, .5/*collision thresh*/, target, false/*keepDistance*/);
        return goToTarget(target);
    }
}

SkillType NaoBehavior::Defender() {

    double y1 = ball.getY()/(ball.getX()+HALF_FIELD_X);
    worldModel->getRVSender()->clearStaticDrawings();
    worldModel->getRVSender()->drawLine(ball.getX()-HALF_FIELD_X, 0, ball.getX(), ball.getY());
    worldModel->getRVSender()->drawCircle(ball.getX(), ball.getY(),2);
    worldModel->getRVSender()->drawCircle(-HALF_FIELD_X, y1, 0.1);
    return  goToTarget(VecPosition( -HALF_FIELD_X+1, y1, 0));

}