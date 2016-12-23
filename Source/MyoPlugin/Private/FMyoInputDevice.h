#pragma once

#include "MyoPluginPrivatePCH.h"
#include <myo/myo.hpp>
#include "IInputDevice.h"

using namespace myo;

class FMyoInputDevice : public IInputDevice, public myo::DeviceListener
{
public:
	TSharedRef< FGenericApplicationMessageHandler > MessageHandler;

	FMyoInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& MessageHandler);
	virtual ~FMyoInputDevice();

	
	/** Tick the interface (e.g. check for new controllers) */
	virtual void Tick(float DeltaTime) override;

	/** Poll for controller state and send events if needed */
	virtual void SendControllerEvents() override;

	void ParseEvents();

	/** Set which MessageHandler will get the events from SendControllerEvents. */
	virtual void SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override;

	/** Exec handler to allow console commands to be passed through for debugging */
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;

	/** IForceFeedbackSystem pass through functions **/
	virtual void SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) override;
	virtual void SetChannelValues(int32 ControllerId, const FForceFeedbackValues &values) override;

	virtual ~FMyoInputDevice();


	TArray<UMyoController*> Controllers;
	TMap<Myo*, UMyoController*> ControllerMap;

	//Custom Private data structure
	//Pass-through functions for plugin methods accessible publicly
	void AddComponentDelegate(UMyoComponent* Component);
	void RemoveComponentDelegate(UMyoComponent* Component);

	//Manipulating the hub, this should be a hub object
	void SetLockingPolicy(EMyoLockingPolicy Policy);
	bool IsHubEnabled();

	//Manipulating the device, this should be on a device object...
	void CalibrateOrientation(int32 MyoId, FRotator Direction);
	void VibrateDevice(int32 MyoId, EMyoVibrationType VibrationType);
	void UnlockDevice(int32 MyoId, EMyoUnlockType UnlockType);
	void LockDevice(int32 MyoId);
	void SetEMGStreamType(int32 MyoId, EMyoStreamEmgType StreamType);

private:
	//Convenience Lambdas
	void RunFunctionOnComponents(TFunction<void(UMyoComponent*)> InFunction);

	//Delegate pointers
	TArray<UMyoComponent*> ComponentDelegates;

	//Myo events, these will be called on the background thread and will require a foreground thread call
	virtual void onPair(Myo* myo, uint64_t timestamp, FirmwareVersion firmwareVersion) override;
	virtual void onUnpair(Myo* myo, uint64_t timestamp) override;
	virtual void onConnect(Myo* myo, uint64_t timestamp, FirmwareVersion firmwareVersion) override;
	virtual void onDisconnect(Myo* myo, uint64_t timestamp) override;
	virtual void onArmSync(Myo* myo, uint64_t timestamp, Arm arm, XDirection xDirection, float rotation, WarmupState warmupState) override;

	virtual void onArmUnsync(Myo* myo, uint64_t timestamp) override;
	virtual void onUnlock(Myo* myo, uint64_t timestamp) override;
	virtual void onLock(Myo* myo, uint64_t timestamp) override;
	virtual void onPose(Myo* myo, uint64_t timestamp, Pose pose) override;
	virtual void onOrientationData(Myo* myo, uint64_t timestamp, const Quaternion<float>& rotation) override;
	virtual void onAccelerometerData(Myo* myo, uint64_t timestamp, const Vector3<float>& accel) override;
	virtual void onGyroscopeData(Myo* myo, uint64_t timestamp, const Vector3<float>& gyro) override;
	virtual void onRssi(Myo* myo, uint64_t timestamp, int8_t rssi) override;
	virtual void onBatteryLevelReceived(myo::Myo* myo, uint64_t timestamp, uint8_t level) override;
	virtual void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg) override;
	virtual void onWarmupCompleted(myo::Myo* myo, uint64_t timestamp, WarmupResult warmupResult) override;
	
	FThreadSafeBool bRunning;

	Hub* MyoHub;

	//Finding and translating connected myos
	//TArray<Myo*> ConnectedMyos;
	//int32 IdForMyo(Myo* myo);
	//Myo* MyoForId(int32 MyoId); 
};