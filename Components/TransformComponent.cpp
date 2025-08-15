#include "TransformComponent.h"

#include "Core/GameObject.h"

using namespace bae;


TransformComponent::TransformComponent(GameObject& owner) :
	Component(owner)
{
}


const glm::vec2& TransformComponent::GetWorldLocation()
{
	if (m_bLocationDirty)
		UpdateWorldLocation();

	return m_WorldLocation;
}

float TransformComponent::GetWorldRotation()
{
	if (m_bRotationDirty)
		UpdateWorldRotation();

	return m_WorldRotation;
}

const glm::vec2& TransformComponent::GetWorldScale()
{
	if (m_bScaleDirty)
		UpdateWorldScale();

	return m_WorldScale;
}


const glm::vec2& TransformComponent::GetLocalLocation() const
{
	return m_LocalLocation;
}

float TransformComponent::GetLocalRotation() const
{
	return m_LocalRotation;
}

const glm::vec2& TransformComponent::GetLocalScale() const
{
	return m_LocalScale;
}


void TransformComponent::SetWorldLocation(const glm::vec2& location)
{
	if (!m_Owner->GetParent())
		m_LocalLocation = location;
	else
		m_LocalLocation = location - m_Owner->GetParent()->GetWorldLocation();

	SetLocationDirty();
}

//void TransformComponent::SetWorldRotation(const glm::vec2& rotation)
void TransformComponent::SetWorldRotation(float rotation)
{
	if (!m_Owner->GetParent())
		m_LocalRotation = rotation;
	else
		m_LocalRotation = rotation - m_Owner->GetParent()->GetWorldRotation();

	SetRotationDirty();
}

void TransformComponent::SetWorldScale(const glm::vec2& scale)
{
	if (!m_Owner->GetParent())
		m_LocalScale = scale;
	else
		m_LocalScale = scale / m_Owner->GetParent()->GetWorldScale();

	SetScaleDirty();
}


void TransformComponent::SetLocalLocation(const glm::vec2& location)
{
	m_LocalLocation = location;
	SetLocationDirty();
}

//void TransformComponent::SetLocalRotation(const glm::vec2& rotation)
void TransformComponent::SetLocalRotation(float rotation)
{
	m_LocalRotation = rotation;
	SetRotationDirty();
}

void TransformComponent::SetLocalScale(const glm::vec2& scale)
{
	m_LocalScale = scale;
	SetScaleDirty();
}


void TransformComponent::AddLocation(const glm::vec2& addLocation)
{
	if (m_bLocationDirty)
		UpdateWorldLocation();

	m_LocalLocation = m_LocalLocation + addLocation;
	SetLocationDirty();
}

void TransformComponent::AddRotation(float addRotation)
{
	if (m_bRotationDirty)
		UpdateWorldRotation();

	m_LocalRotation = m_LocalRotation + addRotation;
	SetRotationDirty();
}

void TransformComponent::AddScale(const glm::vec2& addScale)
{
	if (m_bScaleDirty)
		UpdateWorldScale();

	m_LocalScale = m_LocalScale * addScale;
	SetScaleDirty();
}


void TransformComponent::UpdateWorldLocation()
{
	if (m_bLocationDirty)
	{
		if (!m_Owner->GetParent())
			m_WorldLocation = m_LocalLocation;
		else
			m_WorldLocation = m_Owner->GetParent()->GetWorldLocation() + m_LocalLocation;

	}

	m_bLocationDirty = false;
}

void TransformComponent::UpdateWorldRotation()
{
	if (m_bRotationDirty)
	{
		if (!m_Owner->GetParent())
			m_WorldRotation = m_LocalRotation;
		else
			m_WorldRotation = m_Owner->GetParent()->GetWorldRotation() + m_LocalRotation;

	}

	m_bRotationDirty = false;
}

void TransformComponent::UpdateWorldScale()
{
	if (m_bScaleDirty)
	{
		if (!m_Owner->GetParent())
			m_WorldScale = m_LocalScale;
		else
			m_WorldScale = m_Owner->GetParent()->GetWorldScale() * m_LocalScale;

	}

	m_bScaleDirty = false;
}


