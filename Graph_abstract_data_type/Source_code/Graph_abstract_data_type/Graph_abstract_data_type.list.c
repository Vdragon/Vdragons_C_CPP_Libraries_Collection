/*Graph_abstract_data_type.list.c
-----------------------------------
更新紀錄 | Changelog
  Changelog is now stored on GitHub
已知問題 | Known Issues
  Known issues is now stored on GitHub
待辦事項 | Todos
  Todo is now stored on GitHub
著作權宣告 | Copyright notice
  Copyright 2012 林博仁(Henry Lin, pika1021@gmail.com)
智慧財產授權條款：
  Graph_abstract_data_type.list.c is part of Graph抽象資料結構
  Graph抽象資料結構 is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Graph抽象資料結構 is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with Graph抽象資料結構.  If not, see <http://www.gnu.org/licenses/>.
*/

/*--------------程式碼開始(Code Started)--------------*/
/*--------------前期處理器指令(Preprocessor Directive)--------------*/
/*////////程式所include之函式庫的標頭檔(Included Library Headers)////////*/

/*////////常數與巨集(Constants & Macros)////////*/

/*////////其他前期處理器指令(Other Preprocessor Directives////////*/

/*--------------全域宣告與定義(Global Declaration & Definition)--------------*/
/*////////Classes、資料結構(Data Structures)、type definitions跟enumerations////////*/

/*////////函式雛型(Function Prototypes)////////*/

/*////////全域變數(Global Variables)////////*/

/*--------------主要程式碼(Main Code)--------------*/
/*插入一個邊(edge)至相鄰性List圖中的函式
  版本：1.00(1)
  參數：mode（有無方向性）、target（相鄰性List圖Head陣列）、item（限定非Loop的邊）*/
short int graphListInsertEdge(GraphTypes mode, Graph target, Edge item)
    {
    /*宣告與定義(Declaration & Definition)*/
    /*1.2用的讀取指標*/
    register AdjListNode * current_node_position = NULL;

    /*用來第二次呼叫的邊*/
    Edge exchange_edge;

    /*判斷是否是第二次呼叫的靜態字元*/
    static char second_call = 'n';

    /*－－－－－－－－－－－－－－－－－－－－－*/
    /*0.-如果出現自成loop的邊的話就錯誤退出*/
    if(item.u == item.v){
        return -2;
    }

    /*1.-先處理u頂點*/
    /*1.1-如果說該邊的Head目前沒有點...*/
    if(target[item.u] == NULL){
        /*1.1.1-要求一個節點的記憶體並附加在Head上*/
        target[item.u] = (AdjListNode *)malloc(sizeof(AdjListNode));

        /*1.1.2-如果要求記憶體失敗*/
        if(target[item.u] == NULL){
            return -1;
        }

        /*1.1.3-設定該節點*/
        target[item.u]->connected_vertex = item.v;
        target[item.u]->next = NULL;

        /*如果是無向圖且非第二次呼叫的話就將點交換頂點再呼叫自己一次*/
        if(second_call == 'n' && mode == UNDIRECTED){
            exchange_edge.u = item.v;
            exchange_edge.v = item.u;
            second_call = 'y';
            graphListInsertEdge(UNDIRECTED, target, exchange_edge);
        }
        second_call = 'n';

        /*1.1.4-正常離開*/
        return 0;
    }
    /*1.2-將current_node_position設定為第一個節點*/
    current_node_position = target[item.u];

    /*1.3-當還沒到最後一個節點前一直往前*/
    for(current_node_position = target[item.u];
        1;
        current_node_position = current_node_position->next){
        /*1.3.1-如果發現重複的點就正常離開*/
        if(current_node_position->connected_vertex == item.v){
            return 0;
        }
        /*1.3.2-如果此為最後一個節點就離開迴圈*/
        if(current_node_position->next == NULL){
            break;
        }
    }

    /*要求一個節點的記憶體並附加在最後一個頂點節點上*/
    current_node_position->next = (AdjListNode *)malloc(sizeof(AdjListNode));

    /*如果要求記憶體失敗回傳失敗*/
    if(current_node_position->next == NULL){
        return -1;
    }

    /*設定該新節點*/
    current_node_position->next->connected_vertex = item.v;
    current_node_position->next->next = NULL;

    /*如果是無向圖且非第二次呼叫的話就將點交換頂點再呼叫自己一次*/
    if(item.u != item.v && second_call == 'n' && mode == UNDIRECTED){
        exchange_edge.u = item.v;
        exchange_edge.v = item.u;
        second_call = 'y';
        graphListInsertEdge(UNDIRECTED, target, exchange_edge);
    }

    /*重設second_call*/
    second_call = 'n';

    /*－－－－－－－－－－－－－－－－－－－－－*/
    /*傳回內容*/
    return 0;
    }
/*摧毀相鄰性List圖的函式
  版本：1.00(1)*/
void graphListDestroy(Graph target)
    {
    /*宣告與定義(Declaration & Definition)*/
    /*指向要刪除節點的指標*/
    AdjListNode * curr_destroy_node = NULL;

    /*－－－－－－－－－－－－－－－－－－－－－*/
    /*1-處理每個Head*/
    register unsigned i;
    for(i = 0; i < MAX_ADJ_LIST_SIZE; i++){
        /*1.1-如果Head沒有節點就跳過*/
        if(target[i] == NULL){
            continue;
        }

        /*將curr_destroy_node設定成Head指向的節點*/
        curr_destroy_node = target[i];

        /*1.2-在該Head指向的節點後方的節點尚未釋放之前*/
        while(curr_destroy_node->next != NULL){
            /*1.2.1-在curr_destroy_node指向的下一個節點的next不是NULL時將
                    curr_destroy_node指向下一個節點*/
            for(; curr_destroy_node->next->next != NULL ; curr_destroy_node = curr_destroy_node->next){
                ;
            }
            /*1.2.2-將curr_destroy_node的節點指向的下一個節點釋放*/
            free(curr_destroy_node->next);
            curr_destroy_node->next = NULL;

            /*1.2.3-將curr_destroy_node設定成Head指向的節點*/
            curr_destroy_node = target[i];

            /*debug*/
            /*graphListOutput(target);*/
        }

        /*1.3-將Head指向的節點釋放*/
        free(target[i]);
        target[i] = NULL;

        /*debug*/
        /*graphListOutput(target);*/
    }

    /*－－－－－－－－－－－－－－－－－－－－－*/
    /*離開*/
    return ;
    }

/*輸出相鄰性List圖的函式
  版本：1.00(3)*/
void graphListOutput(const Graph target)
    {
    /*宣告與定義(Declaration & Definition)*/
    /*現在正在讀取node的位置的指標*/
    AdjListNode * curr_read_node_pos = NULL;

    /*－－－－－－－－－－－－－－－－－－－－－*/
    /*從第一個輸出到最後一個Head*/
    register unsigned i;
    for(i = 0; i < MAX_ADJ_LIST_SIZE; i++){
        /*輸出Head*/
        printf("頂點""(Vertex)""%u""與""(links with)", i);

        /*如果沒有*/
        if(target[i] == NULL){
            printf("無頂點相連""(no vertex)。\n");
            continue;
        }

        /*輸出節點*/
        printf("頂點""(vertex)");
        for(curr_read_node_pos = target[i];
            curr_read_node_pos != NULL;
            curr_read_node_pos = curr_read_node_pos->next){
            if(curr_read_node_pos == target[i]){
                printf("%u", curr_read_node_pos->connected_vertex);
            }
            else{
                printf(",%u", curr_read_node_pos->connected_vertex);
            }
        }

        printf("相連。\n");
    }
    /*－－－－－－－－－－－－－－－－－－－－－*/
    /*傳回內容*/
    return ;
    }

/*適用於相鄰性List(Adjacency List)的深度優先搜尋(Depth First Search)函式*/
/*版本：1.00(1)*/
void graphAdjListDFS(const Graph target, const Vertex root)
    {
    /*宣告與定義(Declaration & Definition)*/
    /*用來判斷該Head有無拜訪過的陣列，有為非零、沒有為零*/
    static unsigned visited[MAX_ADJ_LIST_SIZE] = {FALSE};

    /*用來判斷是否為最外層呼叫的靜態變數，假設最外層為１*/
    static unsigned call_level = 1;

    /*用來讀取Node的指標*/*
    AdjListNode * readPtr = NULL;

    /*－－－－－－－－－－－－－－－－－－－－－*/
    /*將root的head標記為已拜訪*/
    visited[root] = TRUE;

    /**/
    printf("%d->", root);

    /**/
    for(readPtr = target[root]; readPtr != NULL; readPtr = readPtr->next){
        /*如果還沒拜訪過該節點*/
        if(visited[readPtr->connected_vertex] != TRUE){
            /*用該節點進行呼叫*/
            call_level++;
            graphAdjListDFS(target, readPtr->connected_vertex);
        }
    }

    if(call_level != 1){
        call_level--;
    }
    else{
        register unsigned i;
        for(i = 0; i < MAX_ADJ_LIST_SIZE; i++){
            visited[i] = FALSE;
        }
        printf("END\n");
    }
    /*－－－－－－－－－－－－－－－－－－－－－*/
    /*離開函式*/
    return ;
    }

/*適用於相鄰性List(Adjacency List)的計算dfn、low值函式
  版本：1.10(2)
  傳入參數：圖target、子頂點、parent頂點
  回傳參數：執行狀態*/
short int graphAdjListDfnLow(Graph target, Vertex child, Vertex parent)
    {
    /*宣告與定義(Declaration & Definition)*/
    /*用來判斷該Head有無拜訪過的陣列，有為非零、沒有為零（改為用dfn值 = -1代替無拜訪過。）*/
    /*static unsigned visited[MAX_ADJ_LIST_SIZE] = {FALSE};*//*被替代*/

    /*用來判斷是否為最外層呼叫的靜態變數，假設最外層為１*/
    static unsigned call_level = 0;

    /*用來保存dfn、low值的靜態陣列*/
    static short int dfn[MAX_ADJ_LIST_SIZE], low[MAX_ADJ_LIST_SIZE];

    /*用來遞增dfn跟low的變數*/
    static int num;

    /*讀取List的指標*/
    AdjListNode * readPtr = NULL;

    /*暫存讀取到的下一個子頂點的頂點*/
    Vertex next_child;

    /*－－－－－－－－－－－－－－－－－－－－－*/
    /*在最外層呼叫時初始化dfn、low值陣列*/
    {
    if(call_level == 0){
        register unsigned i;
        for(i = 0; i < MAX_ADJ_LIST_SIZE; i++){
            dfn[i] = low[i] = -1;
        }
        num = 0;
        call_level = 1;
    }
    }

    /*設定child頂點的dfn值為num，low值先設為跟dfn值一樣，有變再改*/
    dfn[child] = low[child] = num++;

    /*從相鄰性List的第一項至最後一項*/
    for(readPtr = target[child]; readPtr != NULL; readPtr = readPtr->next){
        /*從指標獲取下一個子頂點*/
        next_child = readPtr->connected_vertex;

        /*如果還沒拜訪過下一個子頂點*/
        if(dfn[next_child] == -1){
            /*用下一個子頂點當作子頂點呼叫本身*/
            call_level++;
            graphAdjListDfnLow(target, next_child, child);

            /*如果呼叫完表示說low(u)跟low(w)計算好了嗎？*/
            /*low(child) = min(low[child], low[next_child])*/
            low[child] = MIN_OF_2(low[child], low[next_child]);
        }
        /*如果拜訪過下一個子頂點，下一個子頂點又不是parent頂點的話...*/
        else if(next_child != parent){
            /*
            /*low(child) = min(low(child), dfn(next_child))*/
            low[child] = MIN_OF_2(low[child], dfn[next_child]);
        }
    }

    /*如果不是最外層呼叫就level-1*/
    if(call_level != 1){
        call_level--;
    }
    else{
        /*輸出測試數據*/
        register unsigned j, k;

        for(j = 0; j < MAX_ADJ_LIST_SIZE; ){

            for(k = 0; k < MAX_ADJ_LIST_SIZE; k++){
                /*測試輸出*/
                if(dfn[k] == j && SHOW_DEBUG_MESSAGE == 1){
                    printf("dfn(%u)=%d  low(%u)=%d\n", k, dfn[k], k, low[k]);
                    j++;
                }
                else if(dfn[k] == -1 && SHOW_DEBUG_MESSAGE == 1){
                    printf("dfn(%u)=%d  low(%u)=%d\n", k, dfn[k], k, low[k]);
                    j++;
                }
            }
        }

        /*清除靜態陣列以後才能繼續使用dfnlow函式*/
        register unsigned i;

        for(i = 0; i < MAX_ADJ_LIST_SIZE; i++){
            /*clear*/
            dfn[i] = low[i] = -1;
        }
        num = 0;
    }
    /*－－－－－－－－－－－－－－－－－－－－－*/
    /*傳回執行狀態*/
    return 0;
    }

/*適用於相鄰性List(Adjacency List)的尋找多連結圖元件(Biconnected Component)函式*/
/*版本：0.00(0)*/
short int graphAdjListFBC(Graph target, Vertex child, Vertex parent)
    {
    /*宣告與定義(Declaration & Definition)*/
    /*用來判斷該Head有無拜訪過的陣列，有為非零、沒有為零（改為用dfn值 = -1代替無拜訪過。）*/
    /*static unsigned visited[MAX_ADJ_LIST_SIZE] = {FALSE};*//*被替代*/

    /*用來判斷是否為最外層呼叫的靜態變數，假設最外層為１*/
    static unsigned call_level = 0;

    /*用來保存dfn、low值的靜態陣列*/
    static int dfn[MAX_ADJ_LIST_SIZE], low[MAX_ADJ_LIST_SIZE];

    /*用來遞增dfn跟low的變數*/
    static int num;

    /*讀取List的指標*/*
    AdjListNode * readPtr = NULL;

    /*暫存讀取到的下一個子頂點的頂點*/
    Vertex next_child;

    /*暫時存放stack的edge*/
    StackElement temp_edge;
    temp_edge.u = 0;
    temp_edge.v = 0;

    /*宣告一個堆疊(Stack)*/
    StackElement stack[MAX_STACK_SIZE];

    /*指向堆疊top位置指標或註標(subscript)*/
    int stack_top = -1;

    /*回傳狀態*/
    short int call_result;

    /*－－－－－－－－－－－－－－－－－－－－－*/
    /*在最外層呼叫時初始化dfn、low值陣列*/
    {
    if(call_level == 0){
        register unsigned i;
        for(i = 0; i < MAX_ADJ_LIST_SIZE; i++){
            dfn[i] = low[i] = -1;
        }
        num = 0;
        call_level = 1;
    }
    }

    /*設定child頂點的dfn值為num，low值先設為跟dfn值一樣，有變再改*/
    dfn[child] = low[child] = num++;

    /*從相鄰性List的第一項至最後一項*/
    for(readPtr = target[child]; readPtr != NULL; readPtr = readPtr->next){
        /*從指標獲取下一個子頂點*/
        next_child = readPtr->connected_vertex;

        /**/
        if(parent != next_child && dfn[next_child] < dfn[child]){
            /*將線push至stack中*/
            temp_edge.u = child;
            temp_edge.v = next_child;
            call_result = stackPush(temp_edge, stack, &stack_top);

            /*如果next_child沒有拜訪過*/
            if(dfn[next_child] == -1){
                /*用下一個子頂點當作子頂點呼叫本身*/
                call_level++;
                graphAdjListFBC(target, next_child, child);

                /*如果呼叫完表示說low(u)跟low(w)計算好了嗎？*/
                /*low(child) = min(low[child], low[next_child])
                low[child] = MIN_OF_2(low[child], low[next_child]);*/

                /*如果low(w) >= dfn(u)（發現關節點*/
                if(low[next_child] >= dfn[child]){
                    printf("找到一個新的biconnected component：\n"
                           "Find a new biconnected component:\n");
                    do{
                        /*自stack delete edge*/
                        temp_edge = stackPop(stack, &stack_top);
                        if(temp_edge.u == -1 && temp_edge.v == -1){
                            printf("ERROR!\n");
                            return -1;
                        }
                        printf("<%d,%d>", temp_edge.u, temp_edge.v);

                    }while(!((temp_edge.u == child) && (temp_edge.v == next_child)));
                    putchar('\n');

                }
            }
            else if(next_child != parent){
                low[child] = MIN_OF_2(low[child], dfn[next_child]);
            }


        }
        /*如果拜訪過下一個子頂點，下一個子頂點又不是parent頂點的話...*/
        else if(next_child != parent){
            /**/
            /*low(child) = min(low(child), dfn(next_child))*/
            low[child] = MIN_OF_2(low[child], dfn[next_child]);
        }
    }

    /*如果不是最外層呼叫就level-1*/
    if(call_level != 1){
        call_level--;
    }
    else{

        /*清除靜態陣列以後才能繼續使用dfnlow函式*/
        register unsigned i;

        for(i = 0; i < MAX_ADJ_LIST_SIZE; i++){
            /*clear*/
            dfn[i] = low[i] = -1;
        }
        num = 0;
    }
    /*－－－－－－－－－－－－－－－－－－－－－*/
    /*傳回內容*/
    return 0;
    }
