
float kinect2StartX = 10.1/4;
float kinect2StartH = 4.3;

import java.nio.*;
//Thomas Sanchez Lengeling- http://codigogenerativo.com/
import KinectPV2.KJoint;
import KinectPV2.*;
KinectPV2 kinect;
import processing.net.*;
Server myServer;


byte transmitArray[];

void setup() {
  //size(200, 200);
  // Starts a myServer on port 5204
  myServer = new Server(this, 5204);

  //------------ kinect
  kinect = new KinectPV2(this);
  //enable 3d  with (x,y,z) position
  kinect.enableSkeleton3DMap(true);
  kinect.init();

  transmitArray = new byte[18];
  for (int i = 0; i < 18; i++) {
    transmitArray[i] = 0;
  }
}

void draw() {
  //setSkeletons();
  setTestSkeletons();
  writeSkeletons();
}

void setTestSkeletons() {
  for (int i = 0; i < 18; i++) {
    transmitArray[i] = byte(i);
  }
}

void writeSkeletons() {
  myServer.write(transmitArray);
}

void setSkeletons() {
  ArrayList<KSkeleton> skeletonArray =  kinect.getSkeleton3d();
  // send 3 joints (2 hands, spineShoulder), 3 coords for each (x, y, z), 4 bytes per float
  // byte[] skeletonPoints = new byte[3*3*4];
  //individual JOINTS
  for (int i = 0; i < skeletonArray.size(); i++) {
    KSkeleton skeleton = (KSkeleton) skeletonArray.get(i);
    println("num joints: " + skeleton.getJoints().length);
    if (skeleton.isTracked()) {
      KJoint[] joints = skeleton.getJoints();
      setSkeleton(i, joints);
    } else {
      setNoSkeleton(i);
    }
  }
}

void setSkeleton(int ind, KJoint[] joints) {
  transmitArray[ind*4] = byte(255);
  transmitArray[ind*4 + 1] = getXMap(joints[KinectPV2.JointType_SpineShoulder]);
  transmitArray[ind*4 + 2] = getYMap(joints[KinectPV2.JointType_SpineShoulder]);
}

//byte getXMap(KJoint joint) {
//  return metersTo255X(joint.getX() + kinect2StartX);
//}

//byte getYMap(KJoint joint) {
//  float personH = 0.82 * 1.69; // percent of way to shoulder * average human height
//  float heightToShoulder = kinect2StartH - personH;
//  float distance = sqrt(joint.getZ() * joint.getZ() - heightToShoulder * heightToShoulder);
//  return metersTo255X(distance);
//}

byte[] getJointByteArray(KJoint joint) {
  byte[] skeletonPoints = new byte[12];
  byte[] floatArrayX = float2ByteArray(joint.getX());
  byte[] floatArrayY = float2ByteArray(joint.getY());
  byte[] floatArrayZ = float2ByteArray(joint.getZ());
  for (int j = 0; j < 4; j++) {
    skeletonPoints[j] = floatArrayX[j];
    skeletonPoints[4 + j] = floatArrayY[j];
    skeletonPoints[8 + j] = floatArrayZ[j];
  }
  return skeletonPoints;
}

void setNoSkeleton(int ind) {
  transmitArray[ind] = 0;
  for (int i = 1; i < 3; i++) { 
    transmitArray[ind*4 + i] = byte(255);
  }
}

public static byte [] float2ByteArray (float value) {
  return ByteBuffer.allocate(4).putFloat(value).array();
}