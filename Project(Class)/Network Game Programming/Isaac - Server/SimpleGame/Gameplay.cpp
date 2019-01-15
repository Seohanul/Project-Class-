#include "stdafx.h"
#include "Gameplay.h"

void Gameplay::Enter()
{
	Engine.AddTexture(&TEX::BASIC_BODY, "./Resources/Characters/basic_body.png");
	Engine.AddTexture(&TEX::ISAAC_HEAD, "./Resources/Characters/cain_head.png");
	Engine.AddTexture(&TEX::EXPLOSION, "./Resources/explosion.png");
	Engine.AddTexture(&TEX::TEAR, "./Resources/tear.png");
	size_t SOUND_TEST;
	Engine.AddSound(&SOUND_TEST, "./Resources/Sounds/Main.mp3", true);
	Engine.GetSound(SOUND_TEST).Play();

	size_t NULL_STATE;
	//Actor States
	{
		Engine.AddStatePrototype<NullState>(&NULL_STATE);
		Engine.AddStatePrototype<IdleState>(&ST::IDLE);
		Engine.AddStatePrototype<MoveState>(&ST::MOVE);
		Engine.AddStatePrototype<InAirState>(&ST::IN_AIR, 0.0f);
		Engine.AddStatePrototype<ChargeJumpState>(&ST::CHARGE_JUMP, 1.f, 40'000.f);
		Engine.AddStatePrototype<ChargeSlamState>(&ST::CHARGE_SLAM, 1.f);
		Engine.AddStatePrototype<SlamState>(&ST::SLAM, 200'000.f);
		Engine.AddStatePrototype<ShootState>(&ST::SHOOT, TEX::TEAR, 5.f, 2'000.f);
		Engine.AddStatePrototype<DamagedState>(&ST::DAMAGED, 2.f, 10.f);
	}

	//Actor
	{
		Engine.AddObject(&OBJ::PLAYER);

		auto& ActorDescriptor = Engine.GetDescriptor(OBJ::PLAYER);
		auto& ActorPhysics = Engine.GetPhysics(OBJ::PLAYER);
		auto& ActorGraphics = Engine.GetGraphics(OBJ::PLAYER);

		ActorDescriptor.Type = (ObjectType::Actor);
		ActorDescriptor.Value = (100.f); // 100 HP
		ActorDescriptor.Team = OBJ::PLAYER;
		ActorDescriptor.AddEvent(DescriptorEvent::ValueZeroNegative, []()
		{
			STD cout << "Player1 Died!\n";
			Engine.GetGraphics(OBJ::PLAYER).SetAlpha(0.f);
		});
		ActorPhysics.SetCollision(&Collision::Actor);
		ActorPhysics.Box().SetDimensions({ 0.5f, 0.25f, 1.5f });
		ActorPhysics.SetPosition({ 0.f, 5.f, 0.f });

		Engine.AddSprite(&OBJ::SPRITE::BODY, OBJ::PLAYER);
		Engine.AddSprite(&OBJ::SPRITE::HEAD, OBJ::PLAYER);

		auto& Body = Engine.GetSprite(OBJ::PLAYER, OBJ::SPRITE::BODY);
		auto& Head = Engine.GetSprite(OBJ::PLAYER, OBJ::SPRITE::HEAD);

		ActorPhysics.SetMass(70.f);

		float HeadSize = 1.25f;
		float BodySize = 0.75f;

		Body.SetTexture(TEX::BASIC_BODY);
		Body.SetSize({ BodySize, BodySize });
		Body.SetOffset({ 0.f, 0.f, BodySize * 0.5f - 0.1f });
		Body.SetTotal({ 10, 4 });
		Body.SetDirection(Direction::Down);

		Head.SetTexture(TEX::ISAAC_HEAD);
		Head.SetSize({ HeadSize, HeadSize });
		Head.SetTotal({ 2, 4 });
		Head.SetDirection(Direction::Down);
		Head.SetOffset({ 0.f, 0.f, BodySize * 0.5f + HeadSize * 0.5f });
	}

	size_t PLAYER2;

	//Actor2
	{
		Engine.AddObject(&OBJ::PLAYER2);

		auto& ActorDescriptor = Engine.GetDescriptor(OBJ::PLAYER2);
		auto& ActorPhysics = Engine.GetPhysics(OBJ::PLAYER2);
		auto& ActorGraphics = Engine.GetGraphics(OBJ::PLAYER2);

		ActorDescriptor.Type = (ObjectType::Actor);
		ActorDescriptor.Value = (100.f); // 100 HP
		ActorDescriptor.Team = OBJ::PLAYER2;
		
		ActorDescriptor.AddEvent(DescriptorEvent::ValueZeroNegative, []()
		{
			STD cout << "Player2 Died!\n";
			Engine.GetGraphics(OBJ::PLAYER2).SetAlpha(0.f);
		});

		ActorPhysics.SetCollision(&Collision::Actor);
		ActorPhysics.Box().SetDimensions({ 0.5f, 0.25f, 1.5f });
		ActorPhysics.SetPosition({ 5.f, 0.f, 0.f });

		Engine.AddSprite(&OBJ::SPRITE::BODY, OBJ::PLAYER2);
		Engine.AddSprite(&OBJ::SPRITE::HEAD, OBJ::PLAYER2);

		auto& Body = Engine.GetSprite(OBJ::PLAYER2, OBJ::SPRITE::BODY);
		auto& Head = Engine.GetSprite(OBJ::PLAYER2, OBJ::SPRITE::HEAD);

		ActorPhysics.SetMass(70.f);

		float HeadSize = 1.25f;
		float BodySize = 0.75f;

		Body.SetTexture(TEX::BASIC_BODY);
		Body.SetSize({ BodySize, BodySize });
		Body.SetOffset({ 0.f, 0.f, BodySize * 0.5f - 0.1f });
		Body.SetTotal({ 10, 4 });
		Body.SetDirection(Direction::Down);

		Head.SetTexture(TEX::ISAAC_HEAD);
		Head.SetSize({ HeadSize, HeadSize });
		Head.SetTotal({ 2, 4 });
		Head.SetDirection(Direction::Down);
		Head.SetOffset({ 0.f, 0.f, BodySize * 0.5f + HeadSize * 0.5f });
	}

	size_t PLAYER3;

	//Actor3
	{
		Engine.AddObject(&OBJ::PLAYER3);

		auto& ActorDescriptor = Engine.GetDescriptor(OBJ::PLAYER3);
		auto& ActorPhysics = Engine.GetPhysics(OBJ::PLAYER3);
		auto& ActorGraphics = Engine.GetGraphics(OBJ::PLAYER3);

		ActorDescriptor.Type = (ObjectType::Actor);
		ActorDescriptor.Value = (100.f); // 100 HP
		ActorDescriptor.Team = OBJ::PLAYER3;
		ActorDescriptor.AddEvent(DescriptorEvent::ValueZeroNegative, []()
		{
			STD cout << "Player3 Died!\n";
			Engine.GetGraphics(OBJ::PLAYER3).SetAlpha(0.f);
		});
		ActorPhysics.SetCollision(&Collision::Actor);
		ActorPhysics.Box().SetDimensions({ 0.5f, 0.25f, 1.5f });
		ActorPhysics.SetPosition({ 7.f, 7.f, 0.f });

		Engine.AddSprite(&OBJ::SPRITE::BODY, OBJ::PLAYER3);
		Engine.AddSprite(&OBJ::SPRITE::HEAD, OBJ::PLAYER3);

		auto& Body = Engine.GetSprite(OBJ::PLAYER3, OBJ::SPRITE::BODY);
		auto& Head = Engine.GetSprite(OBJ::PLAYER3, OBJ::SPRITE::HEAD);

		ActorPhysics.SetMass(70.f);

		float HeadSize = 1.25f;
		float BodySize = 0.75f;

		Body.SetTexture(TEX::BASIC_BODY);
		Body.SetSize({ BodySize, BodySize });
		Body.SetOffset({ 0.f, 0.f, BodySize * 0.5f - 0.1f });
		Body.SetTotal({ 10, 4 });
		Body.SetDirection(Direction::Down);

		Head.SetTexture(TEX::ISAAC_HEAD);
		Head.SetSize({ HeadSize, HeadSize });
		Head.SetTotal({ 2, 4 });
		Head.SetDirection(Direction::Down);
		Head.SetOffset({ 0.f, 0.f, BodySize * 0.5f + HeadSize * 0.5f });
	}
	

	//Commands
	{
		float Move = 1'500.f;
		Engine.AddCommand<ForceCommand>(&CMD::MOVE_UP, 0.f, Move, 0.f);
		Engine.AddCommand<ForceCommand>(&CMD::MOVE_DOWN, 0.f, -Move, 0.f);
		Engine.AddCommand<ForceCommand>(&CMD::MOVE_LEFT, -Move, 0.f, 0.f);
		Engine.AddCommand<ForceCommand>(&CMD::MOVE_RIGHT, Move, 0.f, 0.f);

		float SlowMove = 750.f;
		Engine.AddCommand<ForceCommand>(&CMD::SLOW_MOVE_UP, 0.f, SlowMove, 0.f);
		Engine.AddCommand<ForceCommand>(&CMD::SLOW_MOVE_DOWN, 0.f, -SlowMove, 0.f);
		Engine.AddCommand<ForceCommand>(&CMD::SLOW_MOVE_LEFT, -SlowMove, 0.f, 0.f);
		Engine.AddCommand<ForceCommand>(&CMD::SLOW_MOVE_RIGHT, SlowMove, 0.f, 0.f);

		Engine.AddCommand<StateCommand>(&CMD::START_MOVE, ST::MOVE);
		Engine.AddCommand<StateCommand>(&CMD::START_CHARGE_JUMP, ST::CHARGE_JUMP);
		Engine.AddCommand<StateCommand>(&CMD::START_CHARGE_SLAM, ST::CHARGE_SLAM);
		Engine.AddCommand<StateCommand>(&CMD::START_IN_AIR, ST::IN_AIR, ST_CMD::ON_RELEASE | ST_CMD::CHANGE_STATE);
		Engine.AddCommand<StateCommand>(&CMD::START_SHOOT, ST::SHOOT, ST_CMD::ON_PRESS | ST_CMD::PUSH_STATE);
		Engine.AddCommand<StateCommand>(&CMD::END_SHOOT, NULL, ST_CMD::ON_RELEASE | ST_CMD::POP_STATE);

		Engine.AddCommand<FaceCommand>(&CMD::FACE_UP, OBJ::SPRITE::HEAD, Direction::Up);
		Engine.AddCommand<FaceCommand>(&CMD::FACE_DOWN, OBJ::SPRITE::HEAD, Direction::Down);
		Engine.AddCommand<FaceCommand>(&CMD::FACE_LEFT, OBJ::SPRITE::HEAD, Direction::Left);
		Engine.AddCommand<FaceCommand>(&CMD::FACE_RIGHT, OBJ::SPRITE::HEAD, Direction::Right);
	}

	//Actor Input
	{
		/* Adding the Inputs to Player */
		Engine.AddController(OBJ::PLAYER, ST::IDLE);
		Engine.AddController(OBJ::PLAYER, ST::MOVE);
		Engine.AddController(OBJ::PLAYER, ST::CHARGE_JUMP);
		Engine.AddController(OBJ::PLAYER, ST::IN_AIR);
		Engine.AddController(OBJ::PLAYER, ST::SHOOT);

		Engine.AddController(OBJ::PLAYER2, ST::IDLE);
		Engine.AddController(OBJ::PLAYER2, ST::MOVE);
		Engine.AddController(OBJ::PLAYER2, ST::CHARGE_JUMP);
		Engine.AddController(OBJ::PLAYER2, ST::IN_AIR);
		Engine.AddController(OBJ::PLAYER2, ST::SHOOT);

		Engine.AddController(OBJ::PLAYER3, ST::IDLE);
		Engine.AddController(OBJ::PLAYER3, ST::MOVE);
		Engine.AddController(OBJ::PLAYER3, ST::CHARGE_JUMP);
		Engine.AddController(OBJ::PLAYER3, ST::IN_AIR);
		Engine.AddController(OBJ::PLAYER3, ST::SHOOT);

		Controller& IdleInput = Engine.GetController(OBJ::PLAYER, ST::IDLE);

		IdleInput.MapControl('W', CMD::START_MOVE);
		IdleInput.MapControl('A', CMD::START_MOVE);
		IdleInput.MapControl('S', CMD::START_MOVE);
		IdleInput.MapControl('D', CMD::START_MOVE);

		IdleInput.MapControl(VK_SPACE, CMD::START_CHARGE_JUMP);
		IdleInput.MapControl(VK_RIGHT, CMD::START_SHOOT);
		IdleInput.MapControl(VK_LEFT, CMD::START_SHOOT);
		IdleInput.MapControl(VK_UP, CMD::START_SHOOT);
		IdleInput.MapControl(VK_DOWN, CMD::START_SHOOT);

		Controller&  MoveInput = Engine.GetController(OBJ::PLAYER, ST::MOVE);

		MoveInput.MapControl('W', CMD::MOVE_UP);
		MoveInput.MapControl('A', CMD::MOVE_LEFT);
		MoveInput.MapControl('S', CMD::MOVE_DOWN);
		MoveInput.MapControl('D', CMD::MOVE_RIGHT);

		MoveInput.MapControl('W', CMD::FACE_UP);
		MoveInput.MapControl('A', CMD::FACE_LEFT);
		MoveInput.MapControl('S', CMD::FACE_DOWN);
		MoveInput.MapControl('D', CMD::FACE_RIGHT);

		MoveInput.MapControl(VK_SPACE, CMD::START_CHARGE_JUMP);
		MoveInput.MapControl(VK_RIGHT, CMD::START_SHOOT);
		MoveInput.MapControl(VK_LEFT, CMD::START_SHOOT);
		MoveInput.MapControl(VK_UP, CMD::START_SHOOT);
		MoveInput.MapControl(VK_DOWN, CMD::START_SHOOT);

		Controller&  ChargeJump = Engine.GetController(OBJ::PLAYER, ST::CHARGE_JUMP);

		ChargeJump.MapControl('W', CMD::SLOW_MOVE_UP);
		ChargeJump.MapControl('A', CMD::SLOW_MOVE_LEFT);
		ChargeJump.MapControl('S', CMD::SLOW_MOVE_DOWN);
		ChargeJump.MapControl('D', CMD::SLOW_MOVE_RIGHT);

		ChargeJump.MapControl('W', CMD::FACE_UP);
		ChargeJump.MapControl('A', CMD::FACE_LEFT);
		ChargeJump.MapControl('S', CMD::FACE_DOWN);
		ChargeJump.MapControl('D', CMD::FACE_RIGHT);

		ChargeJump.MapControl(VK_SPACE, CMD::START_IN_AIR);

		Controller&  InAirInput = Engine.GetController(OBJ::PLAYER, ST::IN_AIR);
		InAirInput.MapControl(VK_SPACE, CMD::START_CHARGE_SLAM);

		Controller& ShootInput = Engine.GetController(OBJ::PLAYER, ST::SHOOT);
		ShootInput.MapControl('W', CMD::MOVE_UP);
		ShootInput.MapControl('A', CMD::MOVE_LEFT);
		ShootInput.MapControl('S', CMD::MOVE_DOWN);
		ShootInput.MapControl('D', CMD::MOVE_RIGHT);

		ShootInput.MapControl(VK_RIGHT, CMD::FACE_RIGHT);
		ShootInput.MapControl(VK_LEFT, CMD::FACE_LEFT);
		ShootInput.MapControl(VK_UP, CMD::FACE_UP);
		ShootInput.MapControl(VK_DOWN, CMD::FACE_DOWN);

		ShootInput.MapControl(VK_RIGHT, CMD::END_SHOOT);
		ShootInput.MapControl(VK_LEFT, CMD::END_SHOOT);
		ShootInput.MapControl(VK_UP, CMD::END_SHOOT);
		ShootInput.MapControl(VK_DOWN, CMD::END_SHOOT);



		Engine.ChangeState(OBJ::PLAYER, ST::IDLE);

		Controller& IdleInput2 = Engine.GetController(OBJ::PLAYER2, ST::IDLE);

		IdleInput2.MapControl('W', CMD::START_MOVE);
		IdleInput2.MapControl('A', CMD::START_MOVE);
		IdleInput2.MapControl('S', CMD::START_MOVE);
		IdleInput2.MapControl('D', CMD::START_MOVE);

		IdleInput2.MapControl(VK_SPACE, CMD::START_CHARGE_JUMP);
		IdleInput2.MapControl(VK_RIGHT, CMD::START_SHOOT);
		IdleInput2.MapControl(VK_LEFT, CMD::START_SHOOT);
		IdleInput2.MapControl(VK_UP, CMD::START_SHOOT);
		IdleInput2.MapControl(VK_DOWN, CMD::START_SHOOT);

		Controller&  MoveInput2 = Engine.GetController(OBJ::PLAYER2, ST::MOVE);

		MoveInput2.MapControl('W', CMD::MOVE_UP);
		MoveInput2.MapControl('A', CMD::MOVE_LEFT);
		MoveInput2.MapControl('S', CMD::MOVE_DOWN);
		MoveInput2.MapControl('D', CMD::MOVE_RIGHT);

		MoveInput2.MapControl('W', CMD::FACE_UP);
		MoveInput2.MapControl('A', CMD::FACE_LEFT);
		MoveInput2.MapControl('S', CMD::FACE_DOWN);
		MoveInput2.MapControl('D', CMD::FACE_RIGHT);

		MoveInput2.MapControl(VK_SPACE, CMD::START_CHARGE_JUMP);
		MoveInput2.MapControl(VK_RIGHT, CMD::START_SHOOT);
		MoveInput2.MapControl(VK_LEFT, CMD::START_SHOOT);
		MoveInput2.MapControl(VK_UP, CMD::START_SHOOT);
		MoveInput2.MapControl(VK_DOWN, CMD::START_SHOOT);

		Controller&  ChargeJump2 = Engine.GetController(OBJ::PLAYER2, ST::CHARGE_JUMP);

		ChargeJump2.MapControl('W', CMD::SLOW_MOVE_UP);
		ChargeJump2.MapControl('A', CMD::SLOW_MOVE_LEFT);
		ChargeJump2.MapControl('S', CMD::SLOW_MOVE_DOWN);
		ChargeJump2.MapControl('D', CMD::SLOW_MOVE_RIGHT);

		ChargeJump2.MapControl('W', CMD::FACE_UP);
		ChargeJump2.MapControl('A', CMD::FACE_LEFT);
		ChargeJump2.MapControl('S', CMD::FACE_DOWN);
		ChargeJump2.MapControl('D', CMD::FACE_RIGHT);

		ChargeJump2.MapControl(VK_SPACE, CMD::START_IN_AIR);

		Controller&  InAirInput2 = Engine.GetController(OBJ::PLAYER2, ST::IN_AIR);
		InAirInput2.MapControl(VK_SPACE, CMD::START_CHARGE_SLAM);

		Controller& ShootInput2 = Engine.GetController(OBJ::PLAYER2, ST::SHOOT);
		ShootInput2.MapControl('W', CMD::MOVE_UP);
		ShootInput2.MapControl('A', CMD::MOVE_LEFT);
		ShootInput2.MapControl('S', CMD::MOVE_DOWN);
		ShootInput2.MapControl('D', CMD::MOVE_RIGHT);

		ShootInput2.MapControl(VK_RIGHT, CMD::FACE_RIGHT);
		ShootInput2.MapControl(VK_LEFT, CMD::FACE_LEFT);
		ShootInput2.MapControl(VK_UP, CMD::FACE_UP);
		ShootInput2.MapControl(VK_DOWN, CMD::FACE_DOWN);

		ShootInput2.MapControl(VK_RIGHT, CMD::END_SHOOT);
		ShootInput2.MapControl(VK_LEFT, CMD::END_SHOOT);
		ShootInput2.MapControl(VK_UP, CMD::END_SHOOT);
		ShootInput2.MapControl(VK_DOWN, CMD::END_SHOOT);



		Engine.ChangeState(OBJ::PLAYER2, ST::IDLE);

		Controller& IdleInput3 = Engine.GetController(OBJ::PLAYER3, ST::IDLE);

		IdleInput3.MapControl('W', CMD::START_MOVE);
		IdleInput3.MapControl('A', CMD::START_MOVE);
		IdleInput3.MapControl('S', CMD::START_MOVE);
		IdleInput3.MapControl('D', CMD::START_MOVE);

		IdleInput3.MapControl(VK_SPACE, CMD::START_CHARGE_JUMP);
		IdleInput3.MapControl(VK_RIGHT, CMD::START_SHOOT);
		IdleInput3.MapControl(VK_LEFT, CMD::START_SHOOT);
		IdleInput3.MapControl(VK_UP, CMD::START_SHOOT);
		IdleInput3.MapControl(VK_DOWN, CMD::START_SHOOT);

		Controller&  MoveInput3 = Engine.GetController(OBJ::PLAYER3, ST::MOVE);

		MoveInput3.MapControl('W', CMD::MOVE_UP);
		MoveInput3.MapControl('A', CMD::MOVE_LEFT);
		MoveInput3.MapControl('S', CMD::MOVE_DOWN);
		MoveInput3.MapControl('D', CMD::MOVE_RIGHT);

		MoveInput3.MapControl('W', CMD::FACE_UP);
		MoveInput3.MapControl('A', CMD::FACE_LEFT);
		MoveInput3.MapControl('S', CMD::FACE_DOWN);
		MoveInput3.MapControl('D', CMD::FACE_RIGHT);

		MoveInput3.MapControl(VK_SPACE, CMD::START_CHARGE_JUMP);
		MoveInput3.MapControl(VK_RIGHT, CMD::START_SHOOT);
		MoveInput3.MapControl(VK_LEFT, CMD::START_SHOOT);
		MoveInput3.MapControl(VK_UP, CMD::START_SHOOT);
		MoveInput3.MapControl(VK_DOWN, CMD::START_SHOOT);

		Controller&  ChargeJump3 = Engine.GetController(OBJ::PLAYER3, ST::CHARGE_JUMP);

		ChargeJump3.MapControl('W', CMD::SLOW_MOVE_UP);
		ChargeJump3.MapControl('A', CMD::SLOW_MOVE_LEFT);
		ChargeJump3.MapControl('S', CMD::SLOW_MOVE_DOWN);
		ChargeJump3.MapControl('D', CMD::SLOW_MOVE_RIGHT);

		ChargeJump3.MapControl('W', CMD::FACE_UP);
		ChargeJump3.MapControl('A', CMD::FACE_LEFT);
		ChargeJump3.MapControl('S', CMD::FACE_DOWN);
		ChargeJump3.MapControl('D', CMD::FACE_RIGHT);

		ChargeJump3.MapControl(VK_SPACE, CMD::START_IN_AIR);

		Controller&  InAirInput3 = Engine.GetController(OBJ::PLAYER3, ST::IN_AIR);
		InAirInput3.MapControl(VK_SPACE, CMD::START_CHARGE_SLAM);

		Controller& ShootInput3 = Engine.GetController(OBJ::PLAYER3, ST::SHOOT);
		ShootInput3.MapControl('W', CMD::MOVE_UP);
		ShootInput3.MapControl('A', CMD::MOVE_LEFT);
		ShootInput3.MapControl('S', CMD::MOVE_DOWN);
		ShootInput3.MapControl('D', CMD::MOVE_RIGHT);

		ShootInput3.MapControl(VK_RIGHT, CMD::FACE_RIGHT);
		ShootInput3.MapControl(VK_LEFT, CMD::FACE_LEFT);
		ShootInput3.MapControl(VK_UP, CMD::FACE_UP);
		ShootInput3.MapControl(VK_DOWN, CMD::FACE_DOWN);

		ShootInput3.MapControl(VK_RIGHT, CMD::END_SHOOT);
		ShootInput3.MapControl(VK_LEFT, CMD::END_SHOOT);
		ShootInput3.MapControl(VK_UP, CMD::END_SHOOT);
		ShootInput3.MapControl(VK_DOWN, CMD::END_SHOOT);



		Engine.ChangeState(OBJ::PLAYER3, ST::IDLE);
	}

	//Test Obj
	{
		size_t TestObj;

		Engine.AddObject(&TestObj);

		Descriptor& TestDesc = Engine.GetDescriptor(TestObj);

		TestDesc.Type = (ObjectType::Structure);
		TestDesc.Value = (0.2f); //Friction

		Physics& ObjPhysics = Engine.GetPhysics(TestObj);
		ObjPhysics.Box().SetDimensions({ 2.f, 20.f, 20.f });
		ObjPhysics.SetCollision(&Collision::Basic);
		ObjPhysics.SetPosition({ -10.f, 0.f, 0.f });
	}

	{
		size_t TestObj;

		Engine.AddObject(&TestObj);

		Descriptor& TestDesc = Engine.GetDescriptor(TestObj);

		TestDesc.Type = (ObjectType::Structure);
		TestDesc.Value = (0.2f); //Friction

		Physics& ObjPhysics = Engine.GetPhysics(TestObj);
		ObjPhysics.Box().SetDimensions({ 20.f, 2.f, 20.f });
		ObjPhysics.SetCollision(&Collision::Basic);
		ObjPhysics.SetPosition({ 0.f, 9.f, 0.f });
	}

	//Test Obj
	{
		size_t TesObj;

		Engine.AddObject(&TesObj);

		Descriptor& TestDesc = Engine.GetDescriptor(TesObj);

		TestDesc.Type = (ObjectType::Structure);
		TestDesc.Value = (0.2f); //Friction

		Physics& ObjPhysics = Engine.GetPhysics(TesObj);
		ObjPhysics.Box().SetDimensions({ 2.f, 20.f, 20.f });
		ObjPhysics.SetCollision(&Collision::Basic);
		ObjPhysics.SetPosition({ 10.f, 0.f, 0.f });
	}

	{
		size_t TesObj;

		Engine.AddObject(&TesObj);

		Descriptor& TestDesc = Engine.GetDescriptor(TesObj);

		TestDesc.Type = (ObjectType::Structure);
		TestDesc.Value = (0.2f); //Friction

		Physics& ObjPhysics = Engine.GetPhysics(TesObj);
		ObjPhysics.Box().SetDimensions({ 20.f, 2.f, 20.f });
		ObjPhysics.SetCollision(&Collision::Basic);
		ObjPhysics.SetPosition({ 0.f, -9.f, 0.f });
	}
}

void Gameplay::Exit()
{
	Engine.DeleteComponents();
}