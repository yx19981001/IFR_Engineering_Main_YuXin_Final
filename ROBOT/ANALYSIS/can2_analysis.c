#include "can2_analysis.h"

LIFT_POSITION_ENCODER chassis_position_encoder[4]={0};

LIFT_POSITION_ENCODER bulletrotate_position_encoder={0};	//ȡ����ת���

extern CHASSIS_DATA chassis_Data;
//extern BULLETLIFT_MOTOR_DATA bulletlift_Motor_Data[2];
extern BULLETROTATE_DATA BulletRotate_Data;

/******************************************
��������CAN2_Feedback_Analysis
�������ܣ��Ե��̵���Լ���̨����������ݽ���
          �õ���������
������������
��������ֵ����
������������
*******************************************/
u32 t_cm_count=0;
void CAN2_Feedback_Analysis(CanRxMsg *rx_message)
{		
		CAN_Receive(CAN2, CAN_FIFO0, rx_message);//��ȡ����	
		switch(rx_message->StdId)
		{
			 case 0x201:
			{
				Speed_Data_deal(&chassis_Data.lf_wheel_fdbV,rx_message);
				Position_Data_deal_DIV8(&chassis_Data.lf_wheel_fdbP,&chassis_position_encoder[LF],rx_message);
				LostCountFeed(&Error_Check.count[LOST_CM1]);
				break;
			}
			case 0x202:
			{
				t_cm_count++;
				Speed_Data_deal(&chassis_Data.rf_wheel_fdbV,rx_message);
				Position_Data_deal_DIV8(&chassis_Data.rf_wheel_fdbP,&chassis_position_encoder[RF],rx_message);
				LostCountFeed(&Error_Check.count[LOST_CM2]);
				break;
			}
			case 0x203:
			{
				Speed_Data_deal(&chassis_Data.lb_wheel_fdbV,rx_message);
				Position_Data_deal_DIV8(&chassis_Data.lb_wheel_fdbP,&chassis_position_encoder[LB],rx_message);
				LostCountFeed(&Error_Check.count[LOST_CM3]);
				break;
			}
			case 0x204:
			{
				Speed_Data_deal(&chassis_Data.rb_wheel_fdbV,rx_message);
				Position_Data_deal_DIV8(&chassis_Data.rb_wheel_fdbP,&chassis_position_encoder[RB],rx_message);
				LostCountFeed(&Error_Check.count[LOST_CM4]);
				break;
			}
			case 0x205:	//ȡ����ת���
			{
				Speed_Data_deal(&BulletRotate_Data.fdbV,rx_message);
				Position_Data_deal_DIV81(&BulletRotate_Data.fdbP,&bulletrotate_position_encoder,rx_message);
				LostCountFeed(&Error_Check.count[LOST_BULLETROTATE1]);
				
//			  BulletLift_Feedback_Deal(&bulletlift_Motor_Data[BULLETLIFT_FRONTID],rx_message);
//				LostCountFeed(&Error_Check.count[LOST_BULLETLIFT1]);
			  break;
			}
			case 0x206:	//��
			{
//			  BulletLift_Feedback_Deal(&bulletlift_Motor_Data[BULLETLIFT_BACKID],rx_message);
//				LostCountFeed(&Error_Check.count[LOST_BULLETLIFT2]);
			  break;
			}
			 default:
			 break;
		}
}


/****************************************************
�������ƣ�CAN_Chassis_SendMsg
�������ܣ����������ݽ����󷢳�
����������motor_201*******������ǰ���ת��
          motor_202*******������ǰ���ת��
          motor_203*******���������ת��
          motor_204*******�����Һ���ת��
��������ֵ�� ��
�����������ݴ���TxMessage�ṹ������CAN_Transmit����
****************************************************/
void CAN2_Chassis_SendMsg(int motor_201,int motor_202,int motor_203,int motor_204)
{	
		CanTxMsg TxMessage;
	  TxMessage.StdId = 0x200;      //֡IDΪ���������CAN_ID
    TxMessage.IDE = CAN_ID_STD;    //��׼֡
    TxMessage.RTR = CAN_RTR_DATA;  //����֡
    TxMessage.DLC = 0x08;          //֡����Ϊ8
    
    TxMessage.Data[0] =(unsigned char)((motor_201>>8)&0xff);
    TxMessage.Data[1] = (unsigned char)(motor_201&0xff);
    TxMessage.Data[2] =(unsigned char)((motor_202>>8)&0xff);
    TxMessage.Data[3] = (unsigned char)(motor_202&0xff);
    TxMessage.Data[4] =(unsigned char)((motor_203>>8)&0xff);
    TxMessage.Data[5] = (unsigned char)(motor_203&0xff);
    TxMessage.Data[6] =(unsigned char)((motor_204>>8)&0xff);
    TxMessage.Data[7] = (unsigned char)(motor_204&0xff);
		 
    CAN_Transmit(CAN2,&TxMessage);
}


/****************************************************
�������ƣ�CAN2_BulletRotate_SendMsg
�������ܣ���ȡ��������ݽ����󷢳�
����������motor_205*******��һ��ȡ����ת
          motor_206*******Ԥ���ڶ���ȡ�����

��������ֵ�� ��
�����������ݴ���tx_message�ṹ������CAN_Transmit����
****************************************************/
void CAN2_BulletRotate_SendMsg(int16_t motor_205,int16_t motor_206)
{	  
    CanTxMsg tx_message;
    tx_message.StdId = 0x1ff;
    tx_message.IDE = CAN_Id_Standard;//��׼֡
    tx_message.RTR = CAN_RTR_Data;   //����֡
    tx_message.DLC = 0x08;           //֡����Ϊ8
    
    tx_message.Data[0] = (char)(motor_205>>8);
    tx_message.Data[1] = (char)motor_205;
    tx_message.Data[2] = (char)(motor_206>>8);
    tx_message.Data[3] = (char)motor_206;
    tx_message.Data[4] = 0x00;
    tx_message.Data[5] = 0x00;
    tx_message.Data[6] = 0x00;
    tx_message.Data[7] = 0x00;
    
    CAN_Transmit(CAN2,&tx_message);
}

