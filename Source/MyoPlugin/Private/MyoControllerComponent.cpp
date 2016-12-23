#include "MyoPluginPrivatePCH.h"
#include "MyoControllerComponent.h"
#include "Engine.h"
#include "CoreUObject.h"

UMyoControllerComponent::UMyoControllerComponent(const FObjectInitializer &init) : UActorComponent(init)
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = true;	//the component automatically ticks so we don't have to
}

void UMyoControllerComponent::OnRegister()
{
	Super::OnRegister();
	IMyoPlugin::Get().AddComponentDelegate(this);
}

void UMyoControllerComponent::OnUnregister()
{
	IMyoPlugin::Get().RemoveComponentDelegate(this);
	Super::OnUnregister();
}

void UMyoControllerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

//Functions forwards, required implementations
bool UMyoControllerComponent::IsHubEnabled()
{
	return IMyoPlugin::Get().IsHubEnabled();
}

void UMyoControllerComponent::SetLockingPolicy(TEnumAsByte<EMyoLockingPolicy> policy)
{
	return IMyoPlugin::Get().SetLockingPolicy(Policy); 
}

UMyoController* UMyoControllerComponent::LeftMyo()
{
	return nullptr; // MyoLeftMyo();
}

UMyoController* UMyoControllerComponent::RightMyo()
{
	return nullptr; // MyoRightMyo();
}

UMyoController* UMyoControllerComponent::PrimaryMyo()
{
	return nullptr;// MyoPrimaryMyo();
}

void UMyoControllerComponent::ConvertToMyoOrientationSpace(FRotator orientation, FRotator& converted)
{
	return; // MyoConvertToMyoOrientationSpace(orientation, converted);
}


