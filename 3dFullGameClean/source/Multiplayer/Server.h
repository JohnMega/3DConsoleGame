
extern std::vector<class AActor*> actors;
extern bool isBeginPlay;

struct ServerPartData
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
    static const int ConnectionsCount = 5;

    std::vector<std::vector<float>*> receivedClientModelData = std::vector<std::vector<float>*>(ConnectionsCount, NULL);
    std::vector<int> availableClientsTable = std::vector<int>(ConnectionsCount, 0);
    std::vector<AActor*> clientActors = std::vector<AActor*>(ConnectionsCount, NULL);
    AActor* characterActor = NULL;

    SOCKET Connections[ConnectionsCount];
    SOCKET ListenSocket = 0;
    std::string myServerIP;
    int connectionsPtr = 0;
    bool isServerCreate = false;

public:
    ServerPartData()
    {
        for (size_t i = 0; i < ConnectionsCount; ++i)
            Connections[i] = 0;
    }

    void ServerDataCleanup()
    {
        isServerCreate = false;
        myServerIP = "";
        connectionsPtr = 0;

        for (size_t i = 0; i < ConnectionsCount; ++i)
        {
            closesocket(Connections[i]);
            Connections[i] = 0;
        }

        closesocket(ListenSocket);
        ListenSocket = 0;

        for (size_t i = 0; i < receivedClientModelData.size(); ++i)
            receivedClientModelData[i] = NULL;

        for (size_t i = 0; i < availableClientsTable.size(); ++i)
            availableClientsTable[i] = 0;

        int clientActorsSize = clientActors.size();
        for (size_t i = 0; i < clientActorsSize; ++i)
        {
            clientActors.pop_back();
        }

        characterActor = NULL;

        WSACleanup();
    }

} serverPartData;

void SendServerData();
void ApplyServerData()
{
    if (serverPartData.isServerCreate)
    {
        for (size_t i = 0; i < serverPartData.availableClientsTable.size(); ++i)
        {
            if (!serverPartData.availableClientsTable[i]) continue;

            ((Model*)serverPartData.clientActors[i]->GetStaticMesh())->SetCurrentAnimPartIndex((*serverPartData.receivedClientModelData[i])[ServerPartData::ModelDataIndexes::AnimationPartIndex]);

            ((Model*)serverPartData.clientActors[i]->GetStaticMesh())->Rotation({ (*serverPartData.receivedClientModelData[i])[ServerPartData::ModelDataIndexes::DirX], (*serverPartData.receivedClientModelData[i])[ServerPartData::ModelDataIndexes::DirY], 0 });

            serverPartData.clientActors[i]->GetStaticMesh()->SetCentreCoord() = { (*serverPartData.receivedClientModelData[i])[ServerPartData::ModelDataIndexes::CentreCoordX]
                , (*serverPartData.receivedClientModelData[i])[ServerPartData::ModelDataIndexes::CentreCoordY]
                , (*serverPartData.receivedClientModelData[i])[ServerPartData::ModelDataIndexes::CentreCoordZ] };
            serverPartData.clientActors[i]->GetStaticMesh()->UpdateRange();
        }

        SendServerData();
    }
}

void ClientDataHandle()
{
    std::vector<float> receivedClientModelData(6, 0);
    
    while (true)
    {
        if (!serverPartData.isServerCreate) return;
        
        for (size_t i = 0; i < serverPartData.availableClientsTable.size(); ++i)
        {
            if (serverPartData.availableClientsTable[i])
            {
                int recvResult = recv(serverPartData.Connections[i], (char*)receivedClientModelData.data(), receivedClientModelData.size() * sizeof(float), NULL);
                if (recvResult <= 0 || recvResult > receivedClientModelData.size() * sizeof(float)) continue;

                *serverPartData.receivedClientModelData[i] = receivedClientModelData;
            }
        }
    }
}

void SendAvailableClientsTable()
{
    std::vector<int> sendedAvailableTable(serverPartData.availableClientsTable.size(), 0);
    for (size_t i = 0; i < serverPartData.availableClientsTable.size(); ++i)
    {
        if (serverPartData.availableClientsTable[i])
        {
            for (int j = 0; j < serverPartData.availableClientsTable.size(); ++j) sendedAvailableTable[j] = serverPartData.availableClientsTable[j];

            send(serverPartData.Connections[i], (char*)sendedAvailableTable.data(), serverPartData.availableClientsTable.size() * sizeof(int), NULL);
        }
    }
}

void SendModelData()
{
    std::vector<float> sendedModelData(6, 0);
    for (size_t i = 0; i < serverPartData.availableClientsTable.size(); ++i)
    {
        if (serverPartData.availableClientsTable[i])
        {
            sendedModelData[ServerPartData::ModelDataIndexes::CentreCoordX] = ((ACharacterActor*)serverPartData.characterActor)->GetCharacterModel()->GetCentreCoord().x;
            sendedModelData[ServerPartData::ModelDataIndexes::CentreCoordY] = ((ACharacterActor*)serverPartData.characterActor)->GetCharacterModel()->GetCentreCoord().y;
            sendedModelData[ServerPartData::ModelDataIndexes::CentreCoordZ] = ((ACharacterActor*)serverPartData.characterActor)->GetCharacterModel()->GetCentreCoord().z;
            sendedModelData[ServerPartData::ModelDataIndexes::DirX] = camActor.GetCurrentCamera()->GetHorCamDir().x;
            sendedModelData[ServerPartData::ModelDataIndexes::DirY] = camActor.GetCurrentCamera()->GetHorCamDir().y;
            sendedModelData[ServerPartData::ModelDataIndexes::AnimationPartIndex] = ((FantomModel*)((ACharacterActor*)serverPartData.characterActor)->GetCharacterModel())->GetCurrentAnimPartIndex();

            send(serverPartData.Connections[i], (char*)sendedModelData.data(), 24, NULL);

            for (size_t j = 0; j < serverPartData.availableClientsTable.size(); ++j)
            {
                if (j == i || !serverPartData.availableClientsTable[j]) continue;

                sendedModelData[ServerPartData::ModelDataIndexes::CentreCoordX] = serverPartData.clientActors[j]->GetStaticMesh()->GetCentreCoord().x;
                sendedModelData[ServerPartData::ModelDataIndexes::CentreCoordY] = serverPartData.clientActors[j]->GetStaticMesh()->GetCentreCoord().y;
                sendedModelData[ServerPartData::ModelDataIndexes::CentreCoordZ] = serverPartData.clientActors[j]->GetStaticMesh()->GetCentreCoord().z;
                sendedModelData[ServerPartData::ModelDataIndexes::DirX] = ((Model*)serverPartData.clientActors[j]->GetStaticMesh())->GetAnimationInfo().animationDir.x;
                sendedModelData[ServerPartData::ModelDataIndexes::DirY] = ((Model*)serverPartData.clientActors[j]->GetStaticMesh())->GetAnimationInfo().animationDir.y;
                sendedModelData[ServerPartData::ModelDataIndexes::AnimationPartIndex] = ((Model*)serverPartData.clientActors[j]->GetStaticMesh())->GetCurrentAnimPartIndex();

                send(serverPartData.Connections[i], (char*)sendedModelData.data(), 24, NULL);
            }
        }
    }
}

void SendServerData()
{
    SendAvailableClientsTable();
    SendModelData();
}

void ClientDisconnectCheck(int i, SOCKET* sListen)
{
    while (true)
    {
        if (!serverPartData.isServerCreate)
        {
            serverPartData.availableClientsTable[i] = false;
            closesocket(serverPartData.Connections[i]);
            return;
        }

        int iSendResult = send(serverPartData.Connections[i], 0, 0, 0);
        if (iSendResult == SOCKET_ERROR) {
            serverPartData.availableClientsTable[i] = false;
            ((Model*)serverPartData.clientActors[i]->GetStaticMesh())->SetRenderMode(false);
            closesocket(serverPartData.Connections[i]);

            break;
        }
    }
}

void LoadSocketContent()
{
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0)
    {
        std::cout << "Error" << std::endl;
        exit(1);
    }
}

void ServerInfoInit(const char* IP, SOCKADDR_IN& servInfo)
{
    ZeroMemory(&servInfo, sizeof(servInfo));

    if (inet_pton(AF_INET, IP, &servInfo.sin_addr) <= 0)
    {
        std::cout << "Error in IP translation to special numeric format" << '\n';
        WSACleanup();
        exit(1);
    }

    servInfo.sin_port = htons(1111);
    servInfo.sin_family = AF_INET;
}

void InitialServerDataSend(SOCKET newConnection)
{
    char mapNameMsg[256];
    for (int i = 0; i < mapName.size(); ++i) mapNameMsg[i] = mapName[i]; mapNameMsg[mapName.size()] = '\0';
    send(newConnection, mapNameMsg, sizeof(mapNameMsg), NULL);

    char currCharacterModelName[256];
    for (int i = 0; i < modelsManager.GetDefaultModelName().size(); ++i) currCharacterModelName[i] = modelsManager.GetDefaultModelName()[i];
    currCharacterModelName[modelsManager.GetDefaultModelName().size()] = '\0';
    send(newConnection, currCharacterModelName, sizeof(currCharacterModelName), NULL);
}

void ClientModelUpload(SOCKET newConnection)
{
    AddActorToStorage<ABaseActor>(actors, modelsManager.CreateDefaultPlayerModel());
    serverPartData.receivedClientModelData[serverPartData.connectionsPtr % serverPartData.availableClientsTable.size()] = new std::vector<float>(6, 0);
    serverPartData.clientActors[serverPartData.connectionsPtr % serverPartData.availableClientsTable.size()] = actors.back();
    
    if (!isBeginPlay)
        actors.back()->BeginPlay();

    AddActorToStorage<ACubemapActor>(actors, actors.back());

    serverPartData.Connections[serverPartData.connectionsPtr] = newConnection;
    serverPartData.availableClientsTable[serverPartData.connectionsPtr % serverPartData.availableClientsTable.size()] = 1;
}

void ServerCreate()
{
    LoadSocketContent();

    SOCKADDR_IN servInfo;
    ServerInfoInit(serverPartData.myServerIP.c_str(), servInfo);

    serverPartData.ListenSocket = socket(AF_INET, SOCK_STREAM, NULL);
    if (serverPartData.ListenSocket == INVALID_SOCKET)
    {
        std::cout << "Error initialization socket # " << WSAGetLastError() << '\n';
        closesocket(serverPartData.ListenSocket);
        WSACleanup();
        exit(1);
    }

    if (bind(serverPartData.ListenSocket, (SOCKADDR*)&servInfo, sizeof(servInfo)) != 0)
    {
        std::cout << "Error Socket binding to server info. Error # " << WSAGetLastError() << '\n';
        closesocket(serverPartData.ListenSocket);
        WSACleanup();
        exit(1);
    }

    if (listen(serverPartData.ListenSocket, serverPartData.ConnectionsCount) != 0)
    {
        std::cout << "Can't start to listen to. Error # " << WSAGetLastError() << '\n';
        closesocket(serverPartData.ListenSocket);
        WSACleanup();
        exit(1);
    }

    SOCKET newConnection = 0;
    while (true)
    {
        if (serverPartData.availableClientsTable[serverPartData.connectionsPtr % serverPartData.availableClientsTable.size()])
        {
            serverPartData.connectionsPtr++;
            continue;
        }

        newConnection = accept(serverPartData.ListenSocket, NULL, NULL);
        if (newConnection == INVALID_SOCKET) { return; }

        if (newConnection != 0) 
        {
            InitialServerDataSend(newConnection);
            ClientModelUpload(newConnection);

            std::thread* clientDisconnectCheck = new std::thread(ClientDisconnectCheck, serverPartData.connectionsPtr % serverPartData.availableClientsTable.size(), &serverPartData.ListenSocket);
            serverPartData.connectionsPtr = 0;
        }
    }
}