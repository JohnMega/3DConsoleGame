
//#define DEBUG

#define NOTHING_KEY 0
#define ESC_KEY 27
#define MENU_SCROLL_KEY 's'
#define ENTER_KEY '\r'
#define ARROW_LEFT_KEY 0x4B
#define ARROW_RIGHT_KEY 0x4D
#define ARROW_UP_KEY 0x48
#define ARROW_DOWN_KEY 0x50
#define DELETE_KEY 'f'
#define JUMP_KEY ' '

// Light power inc/dec keys
#define INC_LIGHT_POWER_KEY 'i'
#define DEC_LIGHT_POWER_KEY 'o'

// Size inc/dec keys
#define BASE_LENGTH_INC_KEY 'q'
#define BASE_LENGTH_DEC_KEY '1'
#define ADDITIONAL_LENGTH_INC_KEY 't'
#define ADDITIONAL_LENGTH_DEC_KEY '4'

#define BASE_WIDTH_INC_KEY 'e'
#define BASE_WIDTH_DEC_KEY '2'
#define ADDITIONAL_WIDTH_INC_KEY 'y'
#define ADDITIONAL_WIDTH_DEC_KEY '5'

#define HEIGHT_INC_KEY 'r'
#define HEIGHT_DEC_KEY '3'

// Rotation keys
#define X_AXIS_ROTATION_KEY 'z'
#define Y_AXIS_ROTATION_KEY 'x'
#define Z_AXIS_ROTATION_KEY 'c'

#define EXTENSION_CAST_INNER(str) . ## str
#define EXTENSION_CAST(str) EXTENSION_CAST_INNER(str)

#define FILE_EXTENSION_INNER(str, extension) str ## .append(#extension)
#define FILE_EXTENSION(str, extension) FILE_EXTENSION_INNER(str, extension)

#define UPROPERTY_(TypeName, VarName)\
bool isActorHas##TypeName() const noexcept override\
{\
	return true;\
}\
TypeName* Get##TypeName() noexcept override\
{\
	return VarName;\
}\
private:\

#define UPROPERTY(TypeName, VarName) UPROPERTY_(TypeName, VarName)

#define DEFINE_CUBEMAP() \
bool isCubemapActive = false;\
bool isActorHasCubemap() const noexcept override\
{\
	return isCubemapActive;\
}\
void AttachCubemap() override\
{\
	isCubemapActive = true;\
}\

#define IS_ACTOR_HAS_TYPE_(ActorName, TypeName)\
if(ActorName->isActorHas##TypeName())\

#define IS_ACTOR_HAS_TYPE(ActorName,TypeName) IS_ACTOR_HAS_TYPE_(ActorName, TypeName)

#define IS_ACTOR_HAS_NOT_TYPE_(ActorName, TypeName)\
if(!ActorName->isActorHas##TypeName())\

#define IS_ACTOR_HAS_NOT_TYPE(ActorName,TypeName) IS_ACTOR_HAS_NOT_TYPE_(ActorName, TypeName)