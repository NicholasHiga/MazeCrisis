#include "Clock.h"
#include "LogManager.h"
//#include "Vec3.h"
#include "Mat4x4.h"
#include "Quaternion.h"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <stdio.h>

using namespace std;

void testLogFileManager();
void testClock();
//void testVector3();
void testMat4x4();
//void testQuaternion();

int main()
{
	//testLogFileManager();
	//testClock();
	//testVector3();
	testMat4x4();
	//testQuaternion();
	getchar();
}

void testLogFileManager()
{
	printf("Log file manager tests.\n");
	printf("Creating log file with name Log.log and LogPriority::LOG_INFO\n\n");
	LogManager log = LogManager::getInstance();

	printf("Following messages should be printed into Log.txt\n");
	printf("These messages\n");
	printf("should\n");
	printf("all\n");
	printf("be printed\n\n");
	log.info("These messages");
	log.trace("should");
	log.warn("all");
	log.error("be printed");
	printf("Press any key to continue.\n");
	getchar();

	system("cls");
	printf("Changing Log Priority to LOG_WARN\n");
	log.setPriority(LogPriority::LOG_WARN);
	printf("Next two messages should not be printed.\n");
	printf("Message 1\n");
	printf("Message 2\n");
	log.info("Message 1");
	log.trace("Message 2");
	printf("\nNext two messages should be printed.\n");
	printf("Error 1\n");
	printf("Error 2\n\n");
	log.warn("Error 1");
	log.error("Error 2");
	printf("Press any key to continue.\n");
	getchar();
	
	system("cls");
	printf("Changing log name to Log.txt\n\n");
	log.setFilename("Log.txt");
	printf("Press any key to continue.\n");
	getchar();
}

void testClock()
{
	double numSeconds = 0;
	double numMilliseconds = 0;
	int i = 0;

	while (i < 10)
	{
		if (Clock::getSeconds() - numSeconds >= 1)
		{
			i++;
			numSeconds = Clock::getSeconds();
			numMilliseconds = Clock::getMilliseconds();
			system("cls");
			printf("Showing clock count for 10 seconds\n");
			printf("Seconds past: %d\n", i);
			printf("Number of seconds: %f\n", numSeconds);
			printf("Number of milliseconds: %f\n", numMilliseconds);
		}
	}
}

/*void testVector3()
{
	Vec3 v1;
	Vec3 v2(0.5, 2.35, -5.03);
	Vec3 v3(v2);
	printf("         Zero constructor output, v1: %s\n", v1.toString().c_str());
	printf("        Three constructor output, v2: %s\n", v2.toString().c_str());
	printf("Copy constructor, copying v2 into v3: %s\n", v3.toString().c_str());
	v1 = v2;
	printf("               Assignment v2 into v1: %s\n\n", v1.toString()
		.c_str());

	if (v1 == v2)
		printf("Testing v1 == v2. This message should show.\n");
	else
		printf("Testing v1 == v2. This message should NOT show.\n");

	v1 = Vec3(-2, 5, -3);
	v2 = Vec3(20, 6, 214);
	printf("Changed v1 to %s, changed v2 to %s\n", v1.toString().c_str(), 
		v2.toString().c_str());

	if (v1 != v2)
		printf("Testing v1 != v2. This message should show.\n\n");
	else
		printf("Testing v1 != v2. This message should NOT show.\n");

	printf("Testing accessors for v1 and v2: \n( %f, %f, %f ), ( %f, %f, %f )",
		v1.X(), v1.Y(), v1.Z(), v2.X(), v2.Y(), v2.Z());

	v1.X(3);
	v1.Y(5);
	v1.Z(15);
	v2.X(4);
	v2.Y(5);
	v2.Z(6);
	printf("\n\nChanged v1 to %s using setters.\n", v1.toString().c_str());
	printf("Changed v2 to %s using setters.", v2.toString().c_str());

	printf("\nNext set of tests will be compared with manual calculations.\n");
	printf("---------------------------------------------------------------------------------------\n");
	printf("\t\t  |\t\t  Manual Calculation | Programmed Calculation\n");
	printf("---------------------------------------------------------------------------------------\n");
	printf("     v1 Cross v2: |\t\t      ( -45, 42, -5) | %s\n", Vec3::cross(v1, v2).toString().c_str());
	printf("     v2 Cross v1: |\t\t       ( 45, -42, 5) | %s\n", Vec3::cross(v2, v1).toString().c_str());
	printf("       v1 Dot v2: |\t\t                 127 | %f\n", Vec3::dot(v1, v2));
	printf("         v1 + v2: |\t\t       ( 7, 10, 21 ) | %s\n", (v1 + v2).toString().c_str());
	printf("      -2.35 * v1: |\t   ( -7.05, -11.75, -35.25 ) | %s\n", (-2.35 * v1).toString().c_str());
	printf("       5.10 * v2: |\t        ( 20.4, 25.5, 30.6 ) | %s\n", (5.1 * v2).toString().c_str());
	printf("Angle in degrees: | \t\t\t   25.932751 | %f\n", Vec3::angleDegrees(v1, v2));
	printf("Angle in radians: | \t\t\t    0.452612 | %f\n", Vec3::angleRadians(v1, v2));
	printf("   v1 Normalized: | ( 0.186411, 0.310685, 0.932055 ) | %s\n", Vec3::normalize(v1).toString().c_str());
	printf("   v2 Normalized: | ( 0.455842, 0.569803, 0.683763 ) | %s\n", Vec3::normalize(v2).toString().c_str());
	printf("---------------------------------------------------------------------------------------\n");

	printf("\nTesting << operator, printing v1: ");
	cout << v1 << endl;
	printf(" Testing toString(), printing v2: %s\n", v2.toString().c_str());
}*/

void testMat4x4()
{
	Mat4x4 m1;

	printf("Testing default constructor / identity matrix.\n");
	m1.printMatrix();

	printf("\nTesting zero matrix.\n");
	m1.setZero();
	m1.printMatrix();

	m1.setValues(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	printf("\nTesting setValues.\n");
	m1.printMatrix();

	Mat4x4 m2(16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);
	printf("\nTesting 16 value constructor.\n");
	m2.printMatrix();

	if (m1 != m2)
		printf("\nTesting m1 != m2, this message should show.\n");
	else
		printf("\nTesting m1 != m2, this message should NOT show.\n");

	printf("\nTesting assignment operator, setting m2 = m1.\n");
	m2 = m1;
	m1.printMatrix();

	if (m1 == m2)
		printf("\nTesting m1 == m2, this message should show.\n");
	else
		printf("\nTesting m1 == m2, this message should NOT show.\n");

	printf("\nTesting * operator, m1 = m1 * m2. Confirmed correct.\n");
	m1 = m1 * m2;
	m1.printMatrix();

	printf("\nTesting transposing, tranposed the previous matrix.\n");
	m1.transpose();
	m1.printMatrix();

	printf(
		"\nTesting creation of translation matrix, x set to 5, y to 4, z to -10.\n");
	m1.setTranslation(5, 4, -10);
	m1.printMatrix();

	printf(
		"\nTesting creation of scalar matrix, x set to -5.3, y set to 11.5, z to -3.7.\n");
	m1.setScale(-5.3, 11.5, -3.7);
	m1.printMatrix();

	printf(
		"\nTesting creation of rotation matrix 0.5 radians around x-axis.\n");
	m1.setRotationXRadians(0.5);
	m1.printMatrix();

	printf(
		"\nTesting creation of rotation matrix 0.5 radians around y-axis.\n");
	m1.setRotationYRadians(0.5);
	m1.printMatrix();

	printf("\nTesting creation of rotation matrix 0.5 radians around z-axis.\n");
	m1.setRotationZRadians(0.5);
	m1.printMatrix();

	printf("\nTesting creation of rotation matrix 30 degrees around x-axis.\n");
	m1.setRotationXDegrees(30);
	m1.printMatrix();

	printf("\nTesting creation of rotation matrix 30 degrees around y-axis.\n");
	m1.setRotationYDegrees(30);
	m1.printMatrix();

	printf("\nTesting creation of rotation matrix 30 degrees around z-axis.\n");
	m1.setRotationZDegrees(30);
	m1.printMatrix();

	m2.setValues(2, 5, 3, 5,
		4, 6, 6, 3,
		11, 3, 2, -2,
		4, -7, 9, 3);
	printf("\nDeterminant of the following matrix: %f, confirmed correct.\n", 
		Mat4x4::determinant(m2));
	m2.printMatrix();

	printf("\nInverse matrix of above.\n");
	m2.invert();
	m2.printMatrix();
}

/*void testQuaternion()
{
	Quaternion q1;
	Quaternion q2(5.0f, 3.1f, 2.3f, 1.1f);
   // Vec3 v(1, 0, 0);
	//Quaternion q3(90 * M_PI / 180.0f, v);
	Quaternion q4(90, v, false);
	Quaternion q5(20, 30, 10, false);
	Quaternion q6(20 * M_PI / 180.0f, 30 * M_PI / 180.0f, 10 * M_PI / 180.0f);

	printf("Testing quaternion constructors.\n");
	printf("Q1: %s\n", q1.toString().c_str());
	printf("Q2: %s\n", q2.toString().c_str());
	printf("Q3: %s\n", q3.toString().c_str());
	printf("Q4: %s\n", q4.toString().c_str());
	printf("Q5: %s\n", q5.toString().c_str());
	printf("Q6: %s\n", q6.toString().c_str());
	q1 = q3;
	printf("q1 = q3, q1: %s\n\n", q1.toString().c_str());

	if (q1 == q3)
		printf("Testing q1 == q3, this message should show.\n");
	else 
		printf("Testing q1 == q3, this message should NOT show.\n");

	if (q4 == q5)
		printf("Testing q4 == q5, this message should NOT show.\n");
	else
		printf("Testing q4 == q5, this message should show.\n");

	if (q1 != q3)
		printf("Testing q1 != q3, this message should NOT show.\n");
	else
		printf("Testing q1 != q3, this message should show.\n");

	if (q4 != q5)
		printf("Testing q4 != q5, this message should show.\n");
	else
		printf("Testing q4 != q5, this message should NOT show.\n\n");

	printf("\nConversion from euler angle tested within constructor, on q5 
		and q6\n");
	printf("Conversion from rotation and vector tested within constructor, 
		on q3 and q4.\n");

	float yaw, pitch, roll;
	q5.quatToEulerDegrees(yaw, pitch, roll);
	printf("Conversion of q5 to euler angles in degrees.\n");
	printf("\t  yaw: %f\n", yaw); 
	printf("\tpitch: %f\n", pitch);
	printf("\t roll: %f\n", roll);
	
	float angleDegrees;
	Vec3 axis;
	q4.toRotationAndVectorDegrees(angleDegrees, axis);
	printf("Conversion of q4 to axis and rotation.\n");
	printf("\tAngle in degrees: %f\n", angleDegrees);
	printf("\t            Axis: %s\n", axis.toString().c_str());

	q2.normalize();
	printf("\nNormalizing q2: %s\n", q2.toString().c_str());

	printf("\nTesting << operator, printing q1: ");
	cout << q1 << endl;
	printf("Testing toString(), printing q2: %s\n", q2.toString().c_str());
}*/