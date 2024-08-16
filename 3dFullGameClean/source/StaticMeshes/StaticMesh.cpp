
#include "StaticMesh.h"
#include "Maths.h"

COORDS StaticMesh::GetTriangleNormVec()
{
    return { 0,0,0 };
}

float StaticMesh::TriangleSurfaceDotCalc(COORDS cameraCoord, COORDS cameraNormalizeRay, float minDotAndCameraDistValue)
{
    return 0;
}

bool StaticMesh::IsDotInCameraParaboloidRadius(COORDS, COORDS, COORDS, float)
{
    return NULL;
}

bool StaticMesh::IsRender() const noexcept
{
    return isRender;
}

void StaticMesh::SetRenderMode(bool isRender) noexcept
{
    this->isRender = isRender;
}

std::vector<StaticMesh*>& StaticMesh::GetModelPolygons() noexcept
{
    std::vector<StaticMesh*> temp;
    return temp;
}

bool StaticMesh::IsParaboloid() const noexcept
{
    return NULL;
}

bool StaticMesh::ToParab(COORDS, COORDS, COORDS&, float) noexcept
{
    return NULL;
}

Range& StaticMesh::SetRange() noexcept
{
    Range range = { 0, 0, 0, 0, 0, 0 };
    return range;
}

bool StaticMesh::IsDotDisplayAfterFiguresDel(COORDS, COORDS, COORDS&, float, int&) const noexcept
{
    return NULL;
}

bool StaticMesh::IsDotInRotatedFigure(COORDS) const noexcept
{
    return NULL;
}

bool StaticMesh::IsRotatedFigureShine(COORDS, COORDS, int)
{
    return NULL;
}

bool StaticMesh::IsRayCrossRotatedFigure(COORDS, COORDS) const noexcept
{
    return NULL;
}

bool StaticMesh::IsRayCrossNotRotatedFigure(COORDS, COORDS) const
{
    return NULL;
}

bool StaticMesh::IsRayCrossParaboloid(COORDS, COORDS) const
{
    return NULL;
}

COORDS StaticMesh::GetCentreCoord() const noexcept
{
    return { NULL, NULL, NULL };
}

COORDS& StaticMesh::SetCentreCoord() noexcept
{
    COORDS centreCoord = { NULL, NULL, NULL };
    return centreCoord;
}

Range StaticMesh::GetRange() const noexcept
{
    return { 0, 0, 0, 0, 0, 0 };
}

void StaticMesh::UpdateRange(size_t flag) noexcept
{ }

bool StaticMesh::IsRotate() const noexcept
{
    return NULL;
}

void StaticMesh::Rotation(float angle, size_t flag)
{ }

objectType StaticMesh::GetObjType() const noexcept
{
    return objType;
}

int StaticMesh::GetColor() const noexcept
{
    return 9;
}

COORDS StaticMesh::GetColor(COORDS) const
{
    return { 255,255,255 };
}

bool StaticMesh::isObjHasTexture() const noexcept
{
    return false;
}

int& StaticMesh::SetColor() noexcept
{
    int color = NULL;
    return color;
}