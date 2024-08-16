#pragma once

extern bool isBeginPlay;

struct ClientPartData
{
    struct ModelDataIndexes
    {
        static const int CentreCoordX = 0;
        static const int CentreCoordY = 1;
        static const int CentreCoordZ = 2;
        static const int DirX = 3;
        static const int DirY = 4;
        static const int AnimationPartIndex = 5;
    };

public:
    std::vector<std::vector<float>*> receivedServerData = std::vector<std::vector<float>*>(serverPartData.ConnectionsCount, NULL);
    std::vector<AActor*> otherClientActors = std::vector<AActor*>(serverPartData.ConnectionsCount, NULL);
    std::vector<int> availableClientsTable = std::vector<int>(serverPartData.ConnectionsCount, 0);
    AActor* characterActor = NULL;

    std::string serverIP;
    SOCKET Connection = 0;
    bool isConnect = false;
    bool isClientDataReadyForCleanup = false;

public:
    void ClientDataCleanup()
    {
        serverIP = "";
        closesocket(Connection);
        Connection = 0;

        for (size_t i = 0; i < receivedServerData.size(); ++i)
            receivedServerData[i] = NULL;

        for (size_t i = 0; i < availableClientsTable.size(); ++i)
            availableClientsTable[i] = 0;

        int otherClientActorsSize = otherClientActors.size();
        for (size_t i = 0; i < otherClientActorsSize; ++i)
        {
            otherClientActors[i] = NULL;
        }

        characterActor = NULL;

        WSACleanup();
    }

} clientPartData;

void SendClientData();
void ApplyClientData()
{
    if (clientPartData.isConnect)
    {
        for (size_t i = 0; i < clientPartData.availableClientsTable.size(); ++i)
        {
            if (!clientPartData.availableClientsTable[i]) continue;
            if (!((*clientPartData.receivedServerData[i])[ClientPartData::ModelDataIndexes::AnimationPartIndex] >= Model::BACK_ANIM_MODEL_INDEX && (*clientPartData.receivedServerData[i])[ClientPartData::ModelDataIndexes::AnimationPartIndex] <= Model::IDLE_ANIM_MODEL_INDEX)) continue;

            ((Model*)clientPartData.otherClientActors[i]->GetStaticMesh())->SetCurrentAnimPartIndex((*clientPartData.receivedServerData[i])[ClientPartData::ModelDataIndexes::AnimationPartIndex]);

            ((Model*)clientPartData.otherClientActors[i]->GetStaticMesh())->Rotation({ (*clientPartData.receivedServerData[i])[ClientPartData::ModelDataIndexes::DirX], (*clientPartData.receivedServerData[i])[ClientPartData::ModelDataIndexes::DirY], 0 });

            clientPartData.otherClientActors[i]->GetStaticMesh()->SetCentreCoord() = { (*clientPartData.receivedServerData[i])[ClientPartData::ModelDataIndexes::CentreCoordX]
                , (*clientPartData.receivedServerData[i])[ClientPartData::ModelDataIndexes::CentreCoordY]
                , (*clientPartData.receivedServerData[i])[ClientPartData::ModelDataIndexes::CentreCoordZ] };
            clientPartData.otherClientActors[i]->GetStaticMesh()->UpdateRange();
        }

        SendClientData();
    }
}

void RecvAvailableTable()
{
    std::vector<int> receivedAvailableTable(clientPartData.availableClientsTable.size(), 0);
    int recvResult = recv(clientPartData.Connection, (char*)receivedAvailableTable.data(), clientPartData.availableClientsTable.size() * sizeof(int), NULL);
    if (recvResult <= 0 || recvResult > clientPartData.availableClientsTable.size() * sizeof(int)) return;

    for (size_t i = 0; i < clientPartData.availableClientsTable.size(); ++i)
    {
        if (receivedAvailableTable[i] == 1 && !clientPartData.availableClientsTable[i])
        {
            if (clientPartData.otherClientActors[i] == NULL)
            {
                AddActorToStorage<ABaseActor>(actors, modelsManager.CreateDefaultPlayerModel());
                clientPartData.receivedServerData[i] = new std::vector<float>(6, 0);
                clientPartData.otherClientActors[i] = actors.back();

                AddActorToStorage<ACubemapActor>(actors, actors.back());

                if (!isBeginPlay)
                    clientPartData.otherClientActors[i]->BeginPlay();
            }
            else
                ((Model*)clientPartData.otherClientActors[i]->GetStaticMesh())->SetRenderMode(true);
        }

        if (receivedAvailableTable[i] == 0 && clientPartData.otherClientActors[i] != NULL)
        {
            ((Model*)clientPartData.otherClientActors[i]->GetStaticMesh())->SetRenderMode(false);
        }

        if(receivedAvailableTable[i] == 0 || receivedAvailableTable[i] == 1)
            clientPartData.availableClientsTable[i] = receivedAvailableTable[i];
    }
}

void RecvModelsData()
{
    std::vector<float> receivedServerData(6, 0);
    for (size_t i = 0; i < clientPartData.availableClientsTable.size(); ++i)
    {
        if (clientPartData.availableClientsTable[i])
        {
            int recvResult = recv(clientPartData.Connection, (char*)receivedServerData.data(), receivedServerData.size() * sizeof(float), NULL);
            if (recvResult <= 0 || recvResult > receivedServerData.size() * sizeof(float)) continue;

            *clientPartData.receivedServerData[i] = receivedServerData;
        }
    }
}

void ServerDataHandle()
{
    while (true)
    {
        if (clientPartData.isConnect)
        {
            if (clientPartData.characterActor == NULL) continue;

            RecvAvailableTable();
            RecvModelsData();
        }
        else
        {
            return;
        }
    }
}

void SendClientData()
{
    std::vector<float> sendedModelData(6, 0);

    if (clientPartData.isConnect)
    {
        sendedModelData[ClientPartData::ModelDataIndexes::CentreCoordX] = ((ACharacterActor*)clientPartData.characterActor)->GetCharacterModel()->GetCentreCoord().x;
        sendedModelData[ClientPartData::ModelDataIndexes::CentreCoordY] = ((ACharacterActor*)clientPartData.characterActor)->GetCharacterModel()->GetCentreCoord().y;
        sendedModelData[ClientPartData::ModelDataIndexes::CentreCoordZ] = ((ACharacterActor*)clientPartData.characterActor)->GetCharacterModel()->GetCentreCoord().z;
        sendedModelData[ClientPartData::ModelDataIndexes::DirX] = camActor.GetCurrentCamera()->GetHorCamDir().x;
        sendedModelData[ClientPartData::ModelDataIndexes::DirY] = camActor.GetCurrentCamera()->GetHorCamDir().y;
        sendedModelData[ClientPartData::ModelDataIndexes::AnimationPartIndex] = ((FantomModel*)((ACharacterActor*)clientPartData.characterActor)->GetCharacterModel())->GetCurrentAnimPartIndex();

        send(clientPartData.Connection, (char*)sendedModelData.data(), 24, NULL);
    }
}

void ClientConnection()
{
    LoadSocketContent();

    SOCKADDR_IN servInfo;
    ServerInfoInit(clientPartData.serverIP.c_str(), servInfo);

    clientPartData.Connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (clientPartData.Connection == INVALID_SOCKET)
    {
        std::cout << "Error initialization socket # " << WSAGetLastError() << '\n';
        closesocket(clientPartData.Connection);
        WSACleanup();
        exit(1);
    }

    if (connect(clientPartData.Connection, (SOCKADDR*)&servInfo, sizeof(servInfo)) != 0) {
        std::cout << "Error: failed connect to server.\n";
        exit(1);
    }

    clientPartData.isConnect = true;
}