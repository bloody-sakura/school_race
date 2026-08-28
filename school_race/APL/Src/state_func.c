#include "state_func.h"
STATEMODE state_mode =
    {
        .cur_mode = DISABLED,
        .set_mode = DISABLED,
};
float H2D(float height) // 将运动的距离转化成要转角度
{
    return (height/(34*3.14159)*360);
}

float L2D(float lenth) // 将运动的距离转化成要转角度
{
    return (lenth/(44*3.14159)*360);
}

void claw_on() // 打开夹爪
{
    solenoid_on(3,0x01);
}

void claw_off() // 闭合夹爪
{
    solenoid_on(3,0x00);
}
bool DoneSignal = false;                  // 判断是否完成指定模式的标志
bool isDone(float feedback, float target) // 判断电机是否运动到指定位置附近（防抖）
{
    if (fabsf(feedback - target) <= 0.05)
        return true;
    return false;
}

void state_func(STATEMODE statemode) // 负责设定PID的目标值
{
    DJmotor[HEIGHT_MOTOR].MODE_Set=DJ_Position;
    DJmotor[LENTH_MOTOR].MODE_Set=DJ_Position;
    switch (statemode.cur_mode)
    {

    case DISABLED:
    {
        DJmotor[HEIGHT_MOTOR].Begin = 0;
        DJmotor[LENTH_MOTOR].Begin = 0;
        DoneSignal = true;
    }
    break;
    case IDLE:
    {
        DJmotor[HEIGHT_MOTOR].Begin = 1;
        DJmotor[LENTH_MOTOR].Begin = 1;
        DJmotor[HEIGHT_MOTOR].valSet.angle_deg = 0;

        DJmotor[LENTH_MOTOR].valSet.angle_deg = 0;
        if (isDone(DJmotor[0].valNow.angle_deg, 0) && isDone(DJmotor[1].valNow.angle_deg, 0))
            DoneSignal = true;
    }
    break;

    case GROUND_CATCH:
    {
        DJmotor[HEIGHT_MOTOR].valSet.angle_deg = H2D(GROUND_CATCH_HEIGHT);

        DJmotor[LENTH_MOTOR].valSet.angle_deg = L2D(GROUND_CATCH_LENTH);
        if (isDone(DJmotor[HEIGHT_MOTOR].valNow.angle_deg, H2D(GROUND_CATCH_HEIGHT)) && isDone(DJmotor[LENTH_MOTOR].valNow.angle_deg, L2D(GROUND_CATCH_LENTH)))
            DoneSignal = true;
    }
    break;

    case GROUND_LIFT:
    {
        DJmotor[HEIGHT_MOTOR].valSet.angle_deg = H2D(GROUND_LIFT_HEIGHT);
        if (isDone(DJmotor[HEIGHT_MOTOR].valNow.angle_deg, H2D(GROUND_LIFT_HEIGHT)))
            DoneSignal = true;
    }
    break;

    case GROUND_DROP:
    {
        DJmotor[HEIGHT_MOTOR].valSet.angle_deg = H2D(GROUND_DROP_HEIGHT);
        if (isDone(DJmotor[HEIGHT_MOTOR].valNow.angle_deg, H2D(GROUND_DROP_HEIGHT)))
            DoneSignal = true;
    }
    break;

    case SKY_CATCH:
    {
        DJmotor[HEIGHT_MOTOR].valSet.angle_deg = H2D(SKY_CATCH_HEIGHT);

        DJmotor[LENTH_MOTOR].valSet.angle_deg = L2D(SKY_CATCH_LENTH);
        if (isDone(DJmotor[HEIGHT_MOTOR].valNow.angle_deg, H2D(SKY_CATCH_HEIGHT)) && isDone(DJmotor[LENTH_MOTOR].valNow.angle_deg, L2D(SKY_CATCH_LENTH)))
            DoneSignal = true;
    }
    break;

    case SKY_LIFT:
    {
        DJmotor[HEIGHT_MOTOR].valSet.angle_deg = H2D(SKY_LIFT_HEIGHT);
        if (isDone(DJmotor[HEIGHT_MOTOR].valNow.angle_deg, H2D(SKY_LIFT_HEIGHT)))
            DoneSignal = true;
    }
    break;

    case BALL_CATCH:
    {
        DJmotor[HEIGHT_MOTOR].valSet.angle_deg = H2D(BALL_CATCH_HEIGHT);

        DJmotor[LENTH_MOTOR].valSet.angle_deg = L2D(BALL_CATCH_LENTH);

        if (isDone(DJmotor[HEIGHT_MOTOR].valNow.angle_deg, H2D(BALL_CATCH_HEIGHT)) && isDone(DJmotor[LENTH_MOTOR].valNow.angle_deg, L2D(BALL_CATCH_LENTH)))
            DoneSignal = true;
    }
    break;

    case BALL_LIFT:
    {
        DJmotor[HEIGHT_MOTOR].valSet.angle_deg = H2D(BALL_LIFT_HEIGHT);
        if (isDone(DJmotor[HEIGHT_MOTOR].valNow.angle_deg, H2D(BALL_LIFT_HEIGHT)))
            DoneSignal = true;
    }
    break;

    case BALL_DROP:
    {
        DJmotor[HEIGHT_MOTOR].valSet.angle_deg = H2D(GROUND_DROP_HEIGHT);
        if (isDone(DJmotor[HEIGHT_MOTOR].valNow.angle_deg, H2D(GROUND_DROP_HEIGHT)))
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

void state_receive(CAN_RxHeaderTypeDef Rxheader, uint8_t *Rx_data)
{
    if ((Rxheader.IDE != CAN_ID_EXT) ||(Rxheader.ExtId < 0x01010401) || (Rxheader.ExtId > 0x01010408))
    {
        return;
    }

        uint8_t msg = (uint8_t)(Rxheader.ExtId - 0x01010400U); /* 1..8 */
    switch (msg)
    {
    case 1:
    {
        if (Rx_data[0] == 'M' && Rx_data[1] == 1)
            state_mode.set_mode = IDLE;
        if (Rx_data[0] == 'M' && Rx_data[1] == 0)
            state_mode.set_mode = DISABLED;
    }
    break;
    case 2:
    {
        if (Rx_data[0] == 'R' && Rx_data[1] == 1)
            state_mode.set_mode = CLAW_ON;
        if (Rx_data[0] == 'R' && Rx_data[1] == 0)
            state_mode.set_mode = CLAW_OFF;
    }
    break;
    case 3:
    {
        if (Rx_data[0] == 'G' && Rx_data[1] == 'E')
            state_mode.set_mode = GROUND_CATCH;
    }
    break;
    case 4:
    {
        if (Rx_data[0] == 'P' && Rx_data[1] == 'E')
            state_mode.set_mode = GROUND_DROP;
    }
    break;
    case 5:
    {
        if (Rx_data[0] == 'G' && Rx_data[1] == 'S')
            state_mode.set_mode = SKY_CATCH;
    }
    break;
    case 6:
    {
        if (Rx_data[0] == 'T' && Rx_data[1] == 'S')
            state_mode.set_mode = SKY_LIFT;
    }
    break;
    case 7:
    {
        if (Rx_data[0] == 'G' && Rx_data[1] == 'B')
            state_mode.set_mode = BALL_CATCH;
    }
    break;
    case 8:
    {
        if (Rx_data[0] == 'P' && Rx_data[1] == 'B')
            state_mode.set_mode = BALL_DROP;
    }
    break;
    case 9:
    {
        if (Rx_data[0] == 'H' && Rx_data[1] == 'B')
            state_mode.set_mode = BALL_LIFT;
    }
    break;
    case 10:
    {
        if (Rx_data[0] == 'H' && Rx_data[1] == 'E')
            state_mode.set_mode = GROUND_LIFT;
    }
    break;
    }
}
