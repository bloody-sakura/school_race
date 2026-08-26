#include "state_func.h"

float L2D(float lenth) // 将运动的距离转化成要转角度
{
    return (lenth);
}

void claw_on() // 打开夹爪
{
}

void claw_off() // 闭合夹爪
{
}
bool DoneSignal = false;                  // 判断是否完成指定模式的标志
bool isDone(float feedback, float target) // 判断电机是否运动到指定位置附近（防抖）
{
    if (fabsf(feedback - target) <= 0.05)
        return true;
    return false;
}


void state_func(STATEMODE state_mode) // 负责设定PID的目标值
{
    switch (state_mode.cur_mode)
    {
    case IDLE:
    {
        DJmotor[0].posPID.SetVal = 0;

        DJmotor[1].posPID.SetVal = 0;
        if (isDone(DJmotor[0].valNow.angle_deg, 0) && isDone(DJmotor[1].valNow.angle_deg, 0))
            DoneSignal = true;
            Beep_Alarm(3);
    }
    break;

    case GROUND_CATCH:
    {
        DJmotor[0].posPID.SetVal = L2D(GROUND_CATCH_HEIGHT);

        DJmotor[1].posPID.SetVal = GROUND_CATCH_LENTH;
        if (isDone(DJmotor[0].valNow.angle_deg, L2D(GROUND_CATCH_HEIGHT)) && isDone(DJmotor[1].valNow.angle_deg, GROUND_CATCH_LENTH))
            DoneSignal = true;
    }
    break;

    case GROUND_LIFT:
    {
        DJmotor[0].posPID.SetVal = L2D(GROUND_LIFT_HEIGHT);
        if (isDone(DJmotor[0].valNow.angle_deg, L2D(GROUND_LIFT_HEIGHT)))
            DoneSignal = true;
    }
    break;

    case GROUND_DROP:
    {
        DJmotor[0].posPID.SetVal = L2D(GROUND_DROP_HEIGHT);
        if (isDone(DJmotor[0].valNow.angle_deg, L2D(GROUND_DROP_HEIGHT)))
            DoneSignal = true;
    }
    break;

    case SKY_CATCH:
    {
        DJmotor[0].posPID.SetVal = L2D(SKY_CATCH_HEIGHT);

        DJmotor[1].posPID.SetVal = SKY_CATCH_LENTH;
        if (isDone(DJmotor[0].valNow.angle_deg, L2D(SKY_CATCH_HEIGHT)) && isDone(DJmotor[1].valNow.angle_deg, SKY_CATCH_LENTH))
            DoneSignal = true;
    }
    break;

    case SKY_LIFT:
    {
        DJmotor[0].posPID.SetVal = L2D(SKY_LIFT_HEIGHT);
        if (isDone(DJmotor[0].valNow.angle_deg, L2D(SKY_LIFT_HEIGHT)))
            DoneSignal = true;
    }
    break;

    case BALL_CATCH:
    {
        DJmotor[0].posPID.SetVal = L2D(BALL_CATCH_HEIGHT);

        DJmotor[1].posPID.SetVal = BALL_CATCH_LENTH;

        if (isDone(DJmotor[0].valNow.angle_deg, L2D(BALL_CATCH_HEIGHT)) && isDone(DJmotor[1].valNow.angle_deg, BALL_CATCH_LENTH))
            DoneSignal = true;
    }
    break;

    case BALL_LIFT:
    {
        DJmotor[0].posPID.SetVal = L2D(BALL_LIFT_HEIGHT);
        if (isDone(DJmotor[0].valNow.angle_deg, L2D(BALL_LIFT_HEIGHT)))
            DoneSignal = true;
    }
    break;

    case BALL_DROP:
    {
        DJmotor[0].posPID.SetVal = L2D(GROUND_DROP_HEIGHT);
        if (isDone(DJmotor[0].valNow.angle_deg, L2D(GROUND_DROP_HEIGHT)))
            DoneSignal = true;
    }
    break;

    case CLAW_ON:
    {
        claw_on();
        DoneSignal = true;
    }
    break;

    case CLAW_OFF:
    {
        claw_off();
        DoneSignal = true;
    }
    break;
    }
}
