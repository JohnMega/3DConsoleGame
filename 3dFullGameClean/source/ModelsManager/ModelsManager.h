
#include "Maths.h"
#include "Model.h"
#include "FantomModel.h"

class ModelsManager
{
private:
    std::vector<std::string> playerModelsNames;
    std::string defaultModelName;

    std::vector<Model*> loadedModels;
    int currLoadedModelsPtr;

private:
    FantomModel* CreatePlayerFantomModel(const std::string& pModelName) const;

public:
    ModelsManager();

    std::vector<std::string> GetPlayerModelsNames() const noexcept;

    void SetDefaultModelName(const std::string& pModelName);

    std::string GetDefaultModelName() const noexcept;

    int GetCurrentLoadedModelsPtr() const noexcept;

    int& SetCurrentLoadedModelsPtr() noexcept;

    std::vector<Model*> GetLoadedModels() const noexcept;

    void AddModelToLoadedModels(Model* loadedModel) noexcept;

    Model* CreatePlayerModel(const std::string& pModelName) const;

    Model* CreateDefaultPlayerModel() const;

    FantomModel* CreateDefaultPlayerFantomModel() const;
};