
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include <filesystem>
#include "ModelsManager.h"

ModelsManager::ModelsManager() : currLoadedModelsPtr(0)
{
    std::string path = std::experimental::filesystem::current_path().string();
    path += "\\Models";

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        std::string fn;
        fn.insert(0, entry.path().string().c_str() + entry.path().string().find_last_of('\\') + 1);
        
        if (fn.find(".obj") == std::string::npos && fn.find(".mtl") == std::string::npos && fn != "playerstart" && fn != "Error"
            && fn != "arrows" && fn != "circles" && fn != "scales")
            playerModelsNames.push_back(fn);
    }

    defaultModelName = playerModelsNames[0];
}

Model* ModelsManager::CreatePlayerModel(const std::string& pModelName) const
{
    Model* model = new Model;
    model->AddNewAnimationPart(pModelName + "/" + pModelName + "Back");
    model->AddNewAnimationPart(pModelName + "/" + pModelName + "Run");
    model->AddNewAnimationPart(pModelName + "/" + pModelName + "Right");
    model->AddNewAnimationPart(pModelName + "/" + pModelName + "Left");
    model->AddNewAnimationPart(pModelName + "/" + pModelName + "Jump");
    model->AddNewAnimationPart(pModelName + "/" + pModelName + "Idle");

    return model;
}

FantomModel* ModelsManager::CreatePlayerFantomModel(const std::string& pModelName) const
{
    FantomModel* model = new FantomModel;
    model->AddNewAnimationPart(pModelName + "/" + pModelName + "Back");
    model->AddNewAnimationPart(pModelName + "/" + pModelName + "Run");
    model->AddNewAnimationPart(pModelName + "/" + pModelName + "Right");
    model->AddNewAnimationPart(pModelName + "/" + pModelName + "Left");
    model->AddNewAnimationPart(pModelName + "/" + pModelName + "Jump");
    model->AddNewAnimationPart(pModelName + "/" + pModelName + "Idle");
    
    return model;
}

std::vector<std::string> ModelsManager::GetPlayerModelsNames() const noexcept
{
    return playerModelsNames;
}

int ModelsManager::GetCurrentLoadedModelsPtr() const noexcept
{
    return currLoadedModelsPtr;
}

int& ModelsManager::SetCurrentLoadedModelsPtr() noexcept
{
    return currLoadedModelsPtr;
}

std::vector<Model*> ModelsManager::GetLoadedModels() const noexcept
{
    return loadedModels;
}

void ModelsManager::AddModelToLoadedModels(Model* loadedModel) noexcept
{
    loadedModels.push_back(loadedModel);
}

void ModelsManager::SetDefaultModelName(const std::string& pModelName)
{
    for (size_t i = 0; i < playerModelsNames.size(); ++i)
        if (pModelName == playerModelsNames[i])
            defaultModelName = pModelName;
}

std::string ModelsManager::GetDefaultModelName() const noexcept
{
    return defaultModelName;
}

Model* ModelsManager::CreateDefaultPlayerModel() const
{
    return CreatePlayerModel(defaultModelName);
}

FantomModel* ModelsManager::CreateDefaultPlayerFantomModel() const
{
    return CreatePlayerFantomModel(defaultModelName);
}