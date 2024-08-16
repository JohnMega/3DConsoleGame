#pragma once

#include <vector>
#include "GlobalVars.h"

class AActor;
struct COORDS;
struct Range;

class StaticMesh
{
protected:
    objectType objType;
    bool isRender = true;

protected:
    virtual bool IsDotDisplayAfterFiguresDel(COORDS, COORDS, COORDS&, float, int&) const noexcept;

    virtual bool IsDotInRotatedFigure(COORDS) const noexcept;

    virtual bool IsRotatedFigureShine(COORDS, COORDS, int);

    virtual bool IsRayCrossRotatedFigure(COORDS, COORDS) const noexcept;

    virtual bool IsRayCrossNotRotatedFigure(COORDS, COORDS) const;

    virtual bool IsRayCrossParaboloid(COORDS, COORDS) const;

public:
    friend bool IsRayCrossesParallelepiped(const std::vector<AActor*>&, COORDS, COORDS, int, float, COORDS&, int&) noexcept;
    friend bool IsLightRayCrossesParallelepiped(const std::vector<AActor*>&, COORDS, COORDS, int, int, int);

    bool IsRender() const noexcept;

    void SetRenderMode(bool isRender) noexcept;

    virtual float TriangleSurfaceDotCalc(COORDS cameraCoord, COORDS cameraNormalizeRay, float minDotAndCameraDistValue);

    virtual bool IsDotInCameraParaboloidRadius(COORDS, COORDS, COORDS, float);

    virtual COORDS GetTriangleNormVec();

    virtual COORDS GetCentreCoord() const noexcept;

    virtual Range& SetRange() noexcept;

    virtual COORDS& SetCentreCoord() noexcept;

    virtual Range GetRange() const noexcept;

    virtual void UpdateRange(size_t flag = 0) noexcept;

    virtual bool IsRotate() const noexcept;

    virtual std::vector<StaticMesh*>& GetModelPolygons() noexcept;

    virtual bool IsParaboloid() const noexcept;

    virtual bool ToParab(COORDS, COORDS, COORDS&, float) noexcept;

    virtual void Rotation(float angle, size_t flag = 0);

    virtual objectType GetObjType() const noexcept;

    virtual int GetColor() const noexcept;

    virtual COORDS GetColor(COORDS) const;

    virtual bool isObjHasTexture() const noexcept;

    virtual int& SetColor() noexcept;
};