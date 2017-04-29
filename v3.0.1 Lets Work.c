#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    gyro,           sensorGyro)
#pragma config(Sensor, in2,    this,           sensorNone)
#pragma config(Sensor, dgtl1,  claw,           sensorDigitalOut)
#pragma config(Sensor, dgtl2,  hangLock,       sensorDigitalOut)
#pragma config(Sensor, dgtl3,  liftBottom,     sensorTouch)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_4,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           driveLeftFront, tmotorVex393HighSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           driveRightBack, tmotorVex393HighSpeed_MC29, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port3,           liftRightEnc,  tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_4)
#pragma config(Motor,  port4,           liftLeftIn,    tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           liftRightTop,  tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           liftLeftTop,   tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           liftRightIn,   tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           liftLeftEnc,   tmotorVex393HighSpeed_MC29, openLoop, reversed, encoderPort, I2C_3)
#pragma config(Motor,  port9,           driveLeftBack, tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port10,          driveRightFront, tmotorVex393HighSpeed_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)


int releaseThreshold = 510; // needs to be set correctly later
int heightThreshold = 17;
int pickUpSpeed = 90;

//Competition Control and Duration Setting

#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

const short leftButton = 1;
const short centerButton = 2;
const short rightButton = 4;






task liftHolder()
{
	while(1==1)
	{
			motor[liftLeftEnc] = heightThreshold;
			motor[liftLeftIn] = heightThreshold;
			motor[liftLeftTop] = heightThreshold;
			motor[liftRightEnc] = heightThreshold;
			motor[liftRightIn] = heightThreshold;
			motor[liftRightTop] = heightThreshold;
			wait1Msec(50);
	}
}
// Functions Start Here //


//Clear Function//

void clearGyro()
{
	SensorValue(gyro) = 0;
}

void clearDrive()
{
	nMotorEncoder[driveLeftBack] = 0;
	nMotorEncoder[driveRightBack] = 0;

	resetMotorEncoder[driveRightBack];
	resetMotorEncoder[driveLeftBack];

	// I Guess this works...

}

void clearLift()
{
	nMotorEncoder[liftLeftEnc] = 0;
	nMotorEncoder[liftRightEnc] = 0;

	resetMotorEncoder[liftLeftEnc];
	resetMotorEncoder[liftRightEnc];

}

void clearAllEncoder()
{
	clearDrive();
	clearLift();
}

//Hault Function//

void haultDrive( int time )
{
	motor[driveRightFront] = 0;
	motor[driveRightBack] = 0;
	motor[driveLeftFront] = 0;
	motor[driveLeftBack] = 0;
	wait1Msec(time);
}

void haultLift( int time )
{
	motor[liftLeftEnc] = 0;
	motor[liftLeftIn] = 0;
	motor[liftLeftTop] = 0;
	motor[liftRightEnc] = 0;
	motor[liftRightIn] = 0;
	motor[liftRightTop] = 0;
	wait1Msec(time);
}

void haultAll(int time)
{
	haultDrive(time);
	haultLift(time);
}

//Piston Function//

void closeClaw()
{
	SensorValue(claw) = 0;
}

void openClaw()
{
	SensorValue(claw) = 1;
}

void lockHang()
{
	SensorValue(hangLock) = 0;
}

void unlockHang()
{
	SensorValue(hangLock) = 1;
}

#warning "is the pickup speed good?"

void pickUp()
{
	closeClaw();

	motor[liftLeftEnc] = 127;
	motor[liftLeftIn] = 127;
	motor[liftLeftTop] = 127;
	motor[liftRightEnc] = 127;
	motor[liftRightIn] = 127;
	motor[liftRightTop] = 127;
}
//Drive Funnction//




void driveForwardPid(int Encoder, int minSpeed = 20)
{

	double kP = 0.1;
	double kI = 0.01;
	double kD = 0.6;

	int integral;
	int der;

	int error;
	int prevErr;

	int speed;

	int bias = 3;

	while(nMotorEncoder[driveRightBack] < Encoder)
	{

		error = (Encoder - nMotorEncoder[driveRightBack]);


		if(error > 100)
		{
			integral = 0;
		}
		else
		{
			integral += error;
		}

		der = prevErr - error;
		prevErr = error;

		speed = ((int)(error*kp + integral*ki + der*kd) > minSpeed)?(int)(error*kp+integral*ki+der*kd):minSpeed;

			motor[driveRightFront] = speed;
			motor[driveRightBack] = speed;
			motor[driveLeftFront] = speed;
			motor[driveLeftBack] = speed;
	}
}

void driveEnc(int speed, int Encoder)
{
	int bias = 3;
	while (nMotorEncoder[driveLeftBack] < Encoder || nMotorEncoder[driveRightBack] < Encoder)
	{
		if (SensorValue(gyro) > 10)
		{
			motor[driveRightFront] = speed;
			motor[driveRightBack] = speed;
			motor[driveLeftFront] = speed/bias;
			motor[driveLeftBack] = speed/bias;
		}
		else if (SensorValue(gyro) < 10)
		{
			motor[driveRightFront] = speed/bias;
			motor[driveRightBack] = speed/bias;
			motor[driveLeftFront] = speed;
			motor[driveLeftBack] = speed;
		}
		else
		{
			motor[driveRightFront] = speed;
			motor[driveRightBack] = speed;
			motor[driveLeftFront] = speed;
			motor[driveLeftBack] = speed;
		}
	}
}

void driveTime(int speed, int time)
{
	motor[driveRightFront] = speed;
	motor[driveRightBack] = speed;
	motor[driveLeftFront] = speed;
	motor[driveLeftBack] = speed;
	wait1Msec(time);
}


void drive(int rightSide, int leftSide, int time)
{
	motor[driveRightFront] = rightSide;
	motor[driveRightBack] = rightSide;
	motor[driveLeftFront] = leftSide;
	motor[driveLeftBack] = leftSide;
	wait1Msec(time);
}
int error;
void gyroTurn(int degrees, char direction)
{
	int dir = 1;



	if (direction == 'l' || direction == 'L')
	{
		dir = -1;
	}
	else
	{
		//degrees = -degrees;
	}
	double kP = 0.1;
	double kI = 0.00025;
	double kD = 0.3;



	int integral;
	int der;


	int prevErr;

	int speed;

	int bias = 3;

	int minSpeed = 40;

	int stationaryCount = 0;
	bool targetReached = false;

	while(error > 0)
	{

		error = degrees - abs(SensorValue(gyro));


		if(error > 100)
		{
			integral = 0;
		}
		else
		{
			integral += error;
		}

		der = error - prevErr;
		prevErr = error;

		speed = ((int)(error*kp + integral*ki + der*kd) > minSpeed)?(int)(error*kp+integral*ki+der*kd):minSpeed;

		motor[driveRightFront] = -speed*dir;
		motor[driveRightBack] = -speed*dir;
		motor[driveLeftFront] = speed*dir;
		motor[driveLeftBack] = speed*dir;


	}
}

//Lift Function//

void liftTime(int speed, int time)
{
	motor[liftLeftEnc] = speed;
	motor[liftLeftIn] = speed;
	motor[liftLeftTop] = speed;
	motor[liftRightEnc] = speed;
	motor[liftRightIn] = speed;
	motor[liftRightTop] = speed;
	wait1Msec(time);
}

void liftEnc(int speed, int Enc)
{
	while(nMotorEncoder[liftLeftEnc] < Enc || nMotorEncoder[liftRightEnc] < Enc)
	{
		motor[liftLeftEnc] = speed;
		motor[liftLeftIn] = speed;
		motor[liftLeftTop] = speed;
		motor[liftRightEnc] = speed;
		motor[liftRightIn] = speed;
		motor[liftRightTop] = speed;
	}
}

void lift2Bottom()
{
	while(SensorValue(liftBottom) != 1)
	{
		motor[liftLeftEnc] = -107;
		motor[liftLeftIn] = -107;
		motor[liftLeftTop] = -107;
		motor[liftRightEnc] = -107;
		motor[liftRightIn] = -107;
		motor[liftRightTop] = -107;
	}
	clearLift();
}

//Game Specific//

void score(int distance)
{
	while((nMotorEncoder[liftLeftEnc] < releaseThreshold || nMotorEncoder[liftRightEnc] < releaseThreshold )
		&& (nMotorEncoder[driveLeftBack] <distance|| nMotorEncoder[driveRightBack] < distance))
	{
		if(nMotorEncoder[liftLeftEnc] < releaseThreshold-300 || nMotorEncoder[liftRightEnc] < releaseThreshold-200)
		{
			motor[liftLeftEnc] = 127;
			motor[liftLeftIn] = 127;
			motor[liftLeftTop] = 127;
			motor[liftRightEnc] = 127;
			motor[liftRightIn] = 127;
			motor[liftRightTop] = 127;
		}
		else
		{
			openClaw();
			motor[liftLeftEnc] = 0;
			motor[liftLeftIn] = 0;
			motor[liftLeftTop] = 0;
			motor[liftRightEnc] = 0;
			motor[liftRightIn] = 0;
			motor[liftRightTop] = 0;
		}
		if(nMotorEncoder[driveLeftBack] <distance|| nMotorEncoder[driveRightBack] < distance)
		{
			motor[driveRightFront] = -127;
			motor[driveRightBack] = -127;
			motor[driveLeftFront] = -127;
			motor[driveLeftBack] = -127;
		}
		else
		{
			motor[driveRightFront] = 0;
			motor[driveRightBack] = 0;
			motor[driveLeftFront] = 0;
			motor[driveLeftBack] = 0;
		}
	}
	openClaw();
}

void hangFromGround()
{
	motor[driveRightFront] = 127;
	motor[driveRightBack] = 127;
	motor[driveLeftFront] = 127;
	motor[driveLeftBack] = 127;
	wait1Msec(1000);

	while(SensorValue(liftBottom) != 1)
	{
		motor[liftLeftEnc] = -127;
		motor[liftLeftIn] = -127;
		motor[liftLeftTop] = -127;
		motor[liftRightEnc] = -127;
		motor[liftRightIn] = -127;
		motor[liftRightTop] = -127;
	}

	lockHang();



}

// Functions End//


//Wait for Press--------------------------------------------------
void waitForPress()
{
	while(nLCDButtons == 0){}
	wait1Msec(5);
}
//----------------------------------------------------------------

//Wait for Release------------------------------------------------
void waitForRelease()
{
	while(nLCDButtons != 0){}
	wait1Msec(5);
}
//----------------------------------------------------------------
int count = 0;
void pre_auton()
{
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
	// Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
	// bStopTasksBetweenModes = true;
	//Clear LCD
	clearLCDLine(0);
	clearLCDLine(1);
	//Declare count variable to keep track of our choice

	//Loop while center button is not pressed
	bool auton = false;

#warning "true is diabled    false is enabled check this tmmr"

	while(!auton)
	{
		if(bIfiRobotDisabled) // true is diabled false is enabled check this tmmr
		{
			//Switch case that allows the user to choose from 4 different options
			switch(count){
			case 0:
				//Display first choice
				displayLCDCenteredString(0, "Game  ||  Skills");
				displayLCDCenteredString(1, "   Team 3050A   ");
				waitForPress();
				//Increment or decrement "count" based on button press
				if(nLCDButtons == leftButton)
				{
					waitForRelease();
					count = 1;
				}
				else if(nLCDButtons == rightButton)
				{
					waitForRelease();
					count = 100;
				}
				break;
			case 100:
				//Display second choice
				displayLCDCenteredString(0, "  Enter Skills? ");
				displayLCDCenteredString(1, "Enter Back Enter");
				waitForPress();
				//Increment or decrement "count" based on button press
				if(nLCDButtons == leftButton)
				{
					waitForRelease();
					count = 0;
					auton = true;
				}
				else if(nLCDButtons == rightButton)
				{
					waitForRelease();
					count = 0;
					auton = true;

				}
				else if (nLCDButtons == centerButton)
				{
					waitForRelease();
					count = 0;

				}
				break;
			case 1:
				//Display third choice
				displayLCDCenteredString(0, "    Auton 1    ");
				displayLCDCenteredString(1, "<     Enter    >");
				waitForPress();
				//Increment or decrement "count" based on button press
				if(nLCDButtons == leftButton)
				{
					waitForRelease();
					count--;
				}
				else if(nLCDButtons == rightButton)
				{
					waitForRelease();
					count++;
				}
				else if(nLCDButtons == centerButton)
				{
					waitForRelease();
					auton = true;
				}
				break;
			case 2:
				//Display fourth choice
				displayLCDCenteredString(0, "     left     ");
				displayLCDCenteredString(1, "<     Enter    >");
				waitForPress();
				//Increment or decrement "count" based on button press
				if(nLCDButtons == leftButton)
				{
					waitForRelease();
					count--;
				}
				else if(nLCDButtons == rightButton)
				{
					waitForRelease();
					count = 0;
				}
				else if(nLCDButtons == centerButton)
				{
					waitForRelease();
					auton = true;
				}
				break;
			default:
				count = 0;
				break;

			}
		}
		else
		{
			auton = true;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task autonomous()
{
	//Clear LCD
	clearLCDLine(0);
	clearLCDLine(1);
	//Switch Case that actually runs the user choice
	switch(count){
	case 0:
		//If count = 0, run the code correspoinding with choice 1
		displayLCDCenteredString(0, "Prog Skills");
		displayLCDCenteredString(1, "is running!");




		break;
	case 1:
		//If count = 1, run the code correspoinding with choice 2
		displayLCDCenteredString(0, "Autonomous 1");
		displayLCDCenteredString(1, "is running!");

		clearDrive();
		clearGyro();
		clearLift();

		liftEnc(127, 300);
		haultLift(10);
		openClaw();
		haultAll(500);
		lift2Bottom();
		haultAll(500);
		clearDrive();

		driveForwardPid(550, 60);
		haultDrive(100);

		drive(127, -127, 350);
		haultDrive(10);

		drive(-60,60, 50);
		haultDrive(10);

		haultAll(10);
		clearDrive();
		driveForwardPid(850, 60);
		haultDrive(10);

		pickUp();
		haultLift(10);
		clearLift();
		liftEnc(127, 200);
		startTask(liftHolder);

		drive(127, -127, 450);
		haultDrive(10);

		drive(-60, 60, 50);
		haultDrive(10);

		clearDrive();
		driveForwardPid(100,60);
		haultAll(10);
		clearDrive();
		clearLift();

		stopTask(liftHolder);

		score(300);

		haultAll(10);

		/*
		haultDrive(200);
		startTask(liftHolder);

		drive(-127, -127, 350);
		haultDrive(10);

		drive(0, -127, 1100);
		haultDrive(300);

		haultDrive(10);
		clearGyro();
		clearLift();
		stopTask(liftHolder);

		clearLift();
		clearDrive();

		score(400);
		haultAll(100);
		motor[driveLeftBack] = 0;
		motor[driveLeftFront]= 0;
		motor[driveRightBack]= 0;
		motor[driveRightFront] =0;
		lift2Bottom();
*/

//		drive(-127, -127, 200);
	//	haultAll(500);
/*
		clearGyro();
		clearDrive();

		drive(127, -127, 300);
		haultDrive(10);

		driveForwardPid(1400);
		haultDrive(10);
		pickUp();

		startTask(liftHolder);

		drive(-127, 127, 330);
		haultDrive(10);

		stopTask(liftHolder);

		score(400);
		haultAll(10);
*/

		break;
	case 2:
		//If count = 2, run the code correspoinding with choice 3
		displayLCDCenteredString(0, "Left");
		displayLCDCenteredString(1, "is running!");

			clearDrive();
		clearGyro();
		clearLift();

		liftEnc(127, 300);
		haultLift(10);
		openClaw();
		haultAll(500);
		lift2Bottom();
		haultAll(500);
		clearDrive();

		driveForwardPid(1020, 60);
		haultDrive(100);
		pickUp();
		haultLift(10);
		clearLift();
		liftEnc(127, 200);

		haultDrive(200);
		startTask(liftHolder);

		drive(-127, -127, 350);
		haultDrive(10);

		drive(-127, 0, 1100);
		haultDrive(300);

		haultDrive(10);
		clearGyro();
		clearLift();
		stopTask(liftHolder);

		clearLift();
		clearDrive();

		score(400);
		haultAll(100);
		motor[driveLeftBack] = 0;
		motor[driveLeftFront]= 0;
		motor[driveRightBack]= 0;
		motor[driveRightFront] =0;
		lift2Bottom();

		/*drive(0, 127, 200);
		clearDrive();

		closeClaw();

		drive(127, -127, 200);
		clearDrive();

		driveForwardPid(400);
		clearDrive();

		score(400);
		haultAll(10);*/

		break;

	default:
		displayLCDCenteredString(0, "No valid choice");
		displayLCDCenteredString(1, "was made!");
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task usercontrol()
{

	bLCDBacklight = true;									// Turn on LCD Backlight
	string mainBattery, backupBattery;

	while (true)
	{

		clearLCDLine(0);											// Clear line 1 (0) of the LCD
		clearLCDLine(1);											// Clear line 2 (1) of the LCD

		//Display the Primary Robot battery voltage
		displayLCDString(0, 0, "Primary: ");
		sprintf(mainBattery, "%1.2f%c", nImmediateBatteryLevel/1000.0,'V'); //Build the value to be displayed
		displayNextLCDString(mainBattery);

		//Display the Backup battery voltage
		displayLCDString(1, 0, "Backup: ");
		sprintf(backupBattery, "%1.2f%c", BackupBatteryLevel/1000.0, 'V');	//Build the value to be displayed
		displayNextLCDString(backupBattery);

		wait1Msec(10);

		motor[driveRightFront] = vexRT[Ch2];
		motor[driveRightBack] = vexRT[Ch2];
		motor[driveLeftFront] = vexRT[Ch3];
		motor[driveLeftBack] = vexRT[Ch3];


		if(vexRT[Btn8D] == 1)
			SensorValue(hangLock) = 0;
		else if(vexRT[Btn8U] == 1)
			SensorValue(hangLock) = 1;
	//	else if(vexRT(Btn7U) == 1)
		//	hangFromGround();


		if(SensorValue(liftBottom) == 1)
		{
			nMotorEncoder(liftLeftEnc) = 0;
			nMotorEncoder(liftRightEnc) = 0;
		}

		if(vexRT[Btn5D] == 1 || abs(nMotorEncoder[liftRightEnc]) > releaseThreshold || abs(nMotorEncoder[liftLeftEnc]) > releaseThreshold)
		{
			openClaw();
		}
		else if (vexRT[Btn5U] == 1)
		{
			closeClaw();
		}

		if(vexRT[Btn6U] == 1)
		{
			closeClaw();
			motor[liftLeftEnc] = 127;
			motor[liftLeftIn] = 127;
			motor[liftLeftTop] = 127;
			motor[liftRightEnc] = 127;
			motor[liftRightIn] = 127;
			motor[liftRightTop] = 127;
		}
		else if(vexRT[Btn6D] == 1)
		{
			motor[liftLeftEnc] = -127;
			motor[liftLeftIn] = -127;
			motor[liftLeftTop] = -127;
			motor[liftRightEnc] = -127;
			motor[liftRightIn] = -127;
			motor[liftRightTop] = -127;
		}

		else
		{
			motor[liftLeftEnc] = heightThreshold;
			motor[liftLeftIn] = heightThreshold;
			motor[liftLeftTop] = heightThreshold;
			motor[liftRightEnc] = heightThreshold;
			motor[liftRightIn] = heightThreshold;
			motor[liftRightTop] = heightThreshold;
		}
	}
}
