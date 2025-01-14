
#include <RocketBulletEnemy.h>
#include <MessageHandler.h>
#include <GraphicEngine.h>
#include <PhysicsEngine.h>
#include <Util.h>


#include <list>

//Esta clase representa un rocket bullet disparado desde un enemigo. La diferencia con el rocket bullet
//normal es que este cuando colisiona no hace daño ya que esta comprobacion ya se hace desde el cliente enemigo
//que dispara dicho rocket.

RocketBulletEnemy::RocketBulletEnemy(Vec3<float> position, Vec3<float> direction, Vec3<float> rotation) : Entity(-1, NULL, "bala"),
m_position(position), m_direction(direction), m_velocity(65), m_rotation(rotation), radioExplosion(40)
{

	cargarContenido();
}


RocketBulletEnemy::~RocketBulletEnemy()
{
}

void RocketBulletEnemy::inicializar()
{
}

void RocketBulletEnemy::update(Time elapsedTime)
{
	btVector3 aux(m_direction.getX(), m_direction.getY(), m_direction.getZ());
	m_rigidBody->setLinearVelocity(aux*m_velocity);

	btVector3 Point = m_rigidBody->getCenterOfMassPosition();
	m_renderState.updatePositions(Vec3<float>((f32)Point[0], (f32)Point[1], (f32)Point[2]));

	// Set rotation
	vector3df Euler;
	const btQuaternion& TQuat = m_rigidBody->getOrientation();
	quaternion q(TQuat.getX(), TQuat.getY(), TQuat.getZ(), TQuat.getW());
	q.toEuler(Euler);
	Euler *= RADTODEG;

	m_renderState.updateRotations(Vec3<float>(Euler.X, Euler.Y, Euler.Z));
}

void RocketBulletEnemy::handleInput()
{
}

void RocketBulletEnemy::cargarContenido()
{
	m_nodo = GraphicEngine::i().createNode(m_position, Vec3<float>(1, 1, 1), "../media/redTexture.jpg", "");
	m_renderState.setPosition(m_position);
	m_renderState.setRotation(m_rotation);
	m_renderState.setRenderRot(m_rotation);

	m_rigidBody = PhysicsEngine::i().createBoxRigidBody(this, Vec3<float>(1.f, 1.f, 1.f), 1,false);
	btBroadphaseProxy* proxy = m_rigidBody->getBroadphaseProxy();
	proxy->m_collisionFilterGroup = col::Collisions::RocketEnemy;
	proxy->m_collisionFilterMask = col::rocketenemyCollidesWith;
	m_rigidBody->setCollisionFlags(4);
	radioExplosion = 40.f;
}

void RocketBulletEnemy::borrarContenido()
{
	
	
}

void RocketBulletEnemy::handleMessage(const Message & message)
{

	if (message.mensaje == "COLLISION") {
		
		PhysicsEngine::i().removeRigidBody(m_rigidBody);
		EntityManager::i().removeEntity(this);
		GraphicEngine::i().removeNode(m_nodo);
	}
}

bool RocketBulletEnemy::handleTrigger(TriggerRecordStruct * Trigger)
{
	return false;
}

std::string RocketBulletEnemy::getClassName()
{
	return "RocketBulletEnemy";
}

void RocketBulletEnemy::setPosition(const Vec3<float>& pos)
{
}

float RocketBulletEnemy::explosion(Vec3<float> posExplosion, Vec3<float> posCharacter, float radio)
{

	//TODO aqui podriamos dibujar la shape

	return 0;

}
