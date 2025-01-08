#include "bg_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gui_tool.h"
#include "lcd.h"

Node *createNode(int id,char *name, int data,char *unit)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE); // 如果内存分配失败，退出程序
    }
    newNode->id = id;
    newNode->data = data;
    newNode->name = strdup(name); // 使用strdup复制字符串
    newNode->unit = strdup(unit); // 使用strdup复制字符串
    if (newNode->name == NULL || newNode->unit == NULL) {
        // 如果字符串复制失败，释放已分配的内存并退出
        free(newNode->name); // 如果name已经分配了，则释放它
        free(newNode);
        printf("String duplication failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->next = NULL;
    return newNode;
}

void appendNode(BG_List *list, char *name, int data, char *unit)
{
    // // 创建新节点
     Node *newNode = createNode(list->Data.max_id + 1, name, data, unit);

    // 如果链表为空，则新节点成为头节点
    if (list->head == NULL)
    {
        list->head = newNode;
    }
    else
    {
        // 遍历链表到末尾
        Node *current = list->head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        // 将新节点添加到链表末尾
        current->next = newNode;
    }

    // 更新链表的最大id
    list->Data.max_id = newNode->id;
    // // 调用 Reflash 函数更新显示
    //list->Reflash();
}

// 删除具有特定值的节点
void deleteNode(Node **head, int key)
{
    Node *temp = *head, *prev = NULL;
    if (temp != NULL && temp->data == key)
    {
        *head = temp->next;
        free(temp);
        return;
    }
    while (temp != NULL && temp->data != key)
    {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
        return;
    prev->next = temp->next;
    free(temp);
}

// 查找具有特定值的节点
Node *searchNode(Node *head, int key)
{
    Node *current = head;
    while (current != NULL)
    {
        if (current->id == key)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// 修改链表中节点的数据
void updateNode(Node *head, int oldData, int newData)
{
    Node *current = searchNode(head, oldData);
    if (current != NULL)
    {
        current->data = newData;
    }
}

void List_select(uint8_t id, uint8_t min_count)
{

    for (uint16_t x = 0; x < LCD_WIDTH - LCD_WIDTH / 40 - 1; x++)
    {
        for (uint16_t y = 0; y < 16; y++)
        {
            BG_SIM_Lcd.DrawPoint(x, y + (id - min_count) * 16, 0xFFFFFF);
        }
    }
}

uint8_t get_num_bit(uint32_t num)
{
    uint8_t bit_count = 0;
    if (num == 0)
    { // 特殊情况，0是一位数
        bit_count = 1;
    }
    else
    {
        uint32_t temp = num;
        while (temp != 0)
        {
            temp /= 10; // 整除10
            bit_count++;
        }
    }
    return bit_count;
}

void flash_handle(BG_List *list)
{
    
    if(list->Data.isEnter==1)
    list->Data.flash_run_time++;

    if (list->Data.flash_run_time > list->Data.flash_time)
    {
        list->Data.flash_run_time = 0;
    }
    if (list->Data.flash_run_time < list->Data.flash_time / 2)
    {
        list->Data.flash_flag = 1;
    }
    else
    {
        list->Data.flash_flag = 0;
    }
}


void BG_timer_update(BG_List *list){

    if (list->Data.isEnter == 1 && list->Data.current_id <= list->Data.max_id)
    {
        flash_handle(list);
    }
    else
    {
        list->Data.flash_flag = FLASH_DISABLE;
    }
}



uint8_t BG_List_Exit(BG_List *list){

   
    return list->Data.exit_flag;
}

void ShowList(BG_List *list)
{
    if (list == NULL || list->head == NULL)
    {
        printf("list is NULL or list->head is NULL");
        return;
    }


    if (list->Data.isEnter == 1 || list->Data.change_run == 1)
    {

        list->Data.change_run = 0;
        list->Clear(0x000000);

        list->Data.flash_run_time++;
        for (uint16_t x = 0; x < LCD_WIDTH; x++)
        {
            for (uint16_t y = 0; y < 16; y++)
            {
                BG_SIM_Lcd.DrawPoint(x, y, 0xCC7FFF);
            }
        }
         
        BGUI_tool.DrawLine(0, 0, LCD_WIDTH, 0, 0xFFFFFF);
        BGUI_tool.DrawLine(0, 0, 0, 16, 0xFFFFFF);
        BGUI_tool.ShowString(LCD_WIDTH / 2 - (strlen(list->Data.title)) * 4, 1, (uint8_t *)list->Data.title, 0x00);
        BGUI_tool.DrawLine(0, 16, LCD_WIDTH, 16, 0xFFFFFF);
        BGUI_tool.DrawLine(LCD_WIDTH - 1, 0, LCD_WIDTH - 1, 16, 0xFFFFFF);

        BGUI_tool.DrawLine(LCD_WIDTH / 2 - 5 * 4, LCD_HEIGHT - 16, LCD_WIDTH / 2 + 4 * 4 + 2, LCD_HEIGHT - 16, 0xFFFFFF);
        BGUI_tool.DrawLine(LCD_WIDTH / 2 - 5 * 4, LCD_HEIGHT - 16, LCD_WIDTH / 2 - 5 * 4, LCD_HEIGHT, 0xFFFFFF);
        BGUI_tool.ShowString(LCD_WIDTH / 2 - 4 * 4, LCD_HEIGHT - 16, "EXIT", 0xFFFFFF);
        BGUI_tool.DrawLine(LCD_WIDTH / 2 + 4 * 4 + 2, LCD_HEIGHT - 16, LCD_WIDTH / 2 + 4 * 4 + 2, LCD_HEIGHT, 0xFFFFFF);
        BGUI_tool.DrawLine(LCD_WIDTH / 2 - 5 * 4, LCD_HEIGHT - 1, LCD_WIDTH / 2 + 4 * 4 + 2, LCD_HEIGHT - 1, 0xFFFFFF);
        
        Node *current = list->head; // 使用临时指针来遍历链表
        //  if( list->head ==NULL)
     
        while (current != NULL)
        {
           // printf("Data.min_show_count = %d\n", current->id);
            if (current->id - list->Data.min_show_count > 0 && current->id - list->Data.min_show_count <= list->Data.max_show_count)
            {   
               
                uint16_t x = LCD_WIDTH - LCD_WIDTH / 40 - 1 - strlen(current->unit) * 8;
                if (list->Data.current_id == current->id)
                {
                     
                    if (list->Data.flash_flag == FLASH_ON || list->Data.flash_flag == FLASH_DISABLE)
                    {

                        List_select(list->Data.current_id, list->Data.min_show_count);
                        BGUI_tool.ShowString(5, (current->id - list->Data.min_show_count) * 16, (uint8_t *)current->name, 0x00);
                        BGUI_tool.ShowString(x, (current->id - list->Data.min_show_count) * 16, (uint8_t *)current->unit, 0x00);
                        BGUI_tool.ShowNum(x-get_num_bit(current->data)*9,(current->id - list->Data.min_show_count) * 16,current->data,0x00);
                         
                    }
                    else if (list->Data.flash_flag == FLASH_OFF)
                    {
                        BGUI_tool.ShowString(5, (current->id - list->Data.min_show_count) * 16, (uint8_t *)current->name, 0xFFFFFF);
                        BGUI_tool.ShowString(x, (current->id - list->Data.min_show_count) * 16, (uint8_t *)current->unit, 0xFFFFFF);
                        BGUI_tool.ShowNum(x-get_num_bit(current->data)*9,(current->id - list->Data.min_show_count) * 16,current->data,0xFFFFFF);
                    }
                    
                    // printf("%d\n",x-get_num_bit(current->data)*8-4);
                    // printf("%d\n",get_num_bit(current->data)*8-4);
                }
                else if (list->Data.current_id != current->id)
                {
                    BGUI_tool.ShowNum(x-get_num_bit(current->data)*9,(current->id - list->Data.min_show_count) * 16,current->data,0xFFFFFF);
                    BGUI_tool.ShowString(5, (current->id - list->Data.min_show_count) * 16, (uint8_t *)current->name, 0xffffff);
                    BGUI_tool.ShowString(x, (current->id - list->Data.min_show_count) * 16, (uint8_t *)current->unit, 0xffffff);

                    // BGUI_tool.ShowNum(x-get_num_bit(current->data)*8-4,(current->id - list->Data.min_show_count) * 16,current->data,0xffffff);
                }
            }
            if (list->Data.current_id == list->Data.max_id + 1)
            {

                for (uint16_t x = LCD_WIDTH / 2 - 5 * 4; x < LCD_WIDTH / 2 + 4 * 4 + 3; x++)
                {
                    for (uint16_t y = LCD_HEIGHT - 16; y < LCD_HEIGHT; y++)
                    {
                        BG_SIM_Lcd.DrawPoint(x, y, 0xCC7FFF);
                    }
                }
                BGUI_tool.ShowString(LCD_WIDTH / 2 - 4 * 4, LCD_HEIGHT - 16, "EXIT", 0x00);
                printf("%d\n",LCD_HEIGHT - 16);
            }
             
            BGUI_tool.DrawLine(0, (current->id) * 16, 0, (current->id + 1) * 16, 0xFFFFFF);
            BGUI_tool.DrawLine(0, (current->id + 1 - list->Data.min_show_count) * 16, LCD_WIDTH - LCD_WIDTH / 40 - 1, (current->id + 1 - list->Data.min_show_count) * 16, 0xFFFFFF);
            BGUI_tool.DrawLine(LCD_WIDTH - LCD_WIDTH / 40 - 1, (current->id) * 16, LCD_WIDTH - LCD_WIDTH / 40 - 1, (current->id + 1) * 16, 0xFFFFFF);
            BGUI_tool.DrawLine(LCD_WIDTH - 1, (current->id) * 16, LCD_WIDTH - 1, (current->id + 1) * 16, 0xFFFFFF);

            /***************************************************************************slider_BAR******************************************************************/
            uint16_t y_start, y_over;
            for (uint8_t count = 0; count < LCD_WIDTH / 40; count++)
            {

                if ((list->Data.min_show_count + list->Data.max_show_count) == list->Data.max_id)
                {
                    y_start = (list->Data.min_show_count % list->Data.max_show_count) * (LCD_HEIGHT - 16) / 7 + 16;

                    if (list->Data.max_show_count <= list->Data.max_id)
                    {
                        y_over = list->Data.max_id * (LCD_HEIGHT - 16) / list->Data.max_id + 16;
                    }
                    else
                    {
                        y_over = (list->Data.max_id % list->Data.max_show_count + 1) * 16;
                    }
                }
                else
                {

                    y_start = (list->Data.min_show_count % list->Data.max_show_count) * (LCD_HEIGHT - 16) / 7 + 16;
                    y_over = ((list->Data.min_show_count + list->Data.max_show_count) % list->Data.max_id) * (LCD_HEIGHT - 16) / list->Data.max_id + 16;
                    if (list->Data.max_show_count <= list->Data.max_id)
                    {
                        y_over = ((list->Data.min_show_count + list->Data.max_show_count) % list->Data.max_id) * (LCD_HEIGHT - 16) / list->Data.max_id + 16;
                    }
                    else
                    {
                        y_over = (list->Data.max_id % list->Data.max_show_count + 1) * 16;
                    }
                }

                BGUI_tool.DrawLine(LCD_WIDTH - count - 2, y_start, LCD_WIDTH - count - 2, y_over, 0xFFFFFF);
            }
            /***************************************************************************slider_BAR******************************************************************/
           
            current = current->next; // 移动到下一个节点
        }
        list->Reflash();
        list->Data.last_id = list->Data.current_id;
    }
}

// 释放链表内存
void freeList(Node *head)
{
    Node *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp); 
    }
}
void Select_up(BG_List *list)
{

    if (list->Data.isEnter == 0)
    {
        if (list->Data.current_id == list->Data.max_id + 1)
        {
            list->Data.current_id = 1;
        }
        else
        {
            list->Data.current_id += 1;
        }
    }else{

        Node *current = searchNode(list->head, list->Data.current_id );
        if (current != NULL)
        {
            current->data += 1;
        }
    }
    if (list->Data.current_id != list->Data.max_id + 1)
    {
        if (list->Data.current_id <= list->Data.max_show_count)
        {
            list->Data.min_show_count = 0;
        }
        else
        {
            list->Data.min_show_count = list->Data.current_id - list->Data.max_show_count;
        }
    }
    list->Data.change_run = 1;
    list->Reflash();
}

void Select_Enter(BG_List *list)
{
    list->Data.last_id = list->Data.current_id;
    if(list->Data.current_id==list->Data.max_id+1){
        list->Data.exit_flag = 1;
        list->Clear(0x00);
    }
     
    if (list->Data.isEnter == 1)
    {
        list->Data.isEnter = 0;
        list->Data.flash_flag = 1;
    }
    else if (list->Data.isEnter == 0)
    {
        list->Data.isEnter = 1;
    }
    list->Data.change_run = 1;
    list->Reflash();
}

void Select_down(BG_List *list)
{
    if (list->Data.isEnter == 0)
    {
        if (list->Data.current_id == 1)
        {
            list->Data.current_id = list->Data.max_id + 1;
        }
        else
        {
            list->Data.current_id -= 1;
        }
    }else{
        Node *current = searchNode(list->head, list->Data.current_id );
        if (current != NULL)
        {
            current->data -= 1;
        }

    }
    if (list->Data.current_id != list->Data.max_id + 1)
    {
        if (list->Data.current_id <= list->Data.max_show_count)
        {
            list->Data.min_show_count = 0;
        }
        else
        {
            list->Data.min_show_count = list->Data.current_id - list->Data.max_show_count;
        }
    }
    list->Data.change_run = 1;
    list->Reflash();
}

BG_List* BG_List_Init(char *title, void (*update)(void), void (*clear)(uint32_t))
{
    BG_List* bg_list = (BG_List*)malloc(sizeof(BG_List));
    if (bg_list == NULL) {
        // 内存申请失败
         printf("fail\n");
        return NULL;
    }
     printf("Init\n");
    // 初始化结构体成员
    bg_list->Append = appendNode; // 假设Append函数指针会在之后被赋值
    bg_list->Delete = deleteNode; // 假设Delete函数指针会在之后被赋值
    bg_list->Show = ShowList;   // 假设Show函数指针会在之后被赋值
    bg_list->Up = Select_up;     // 假设Up函数指针会在之后被赋值
    bg_list->Down = Select_down;   // 假设Down函数指针会在之后被赋值
    bg_list->Enter = Select_Enter;  // 假设Enter函数指针会在之后被赋值
    bg_list->Exit = BG_List_Exit;
    bg_list->Reflash = update; // 使用传入的update函数指针
    bg_list->Clear = clear;     // 使用传入的clear函数指针
    bg_list->Timer_update = BG_timer_update; // 假设Timer_update函数指针会在之后被赋值
    bg_list->head = NULL; // 初始化链表头指针

    bg_list->Data.title = title;
    bg_list->Data.current_id = 4;
    bg_list->Data.isEnter = 0;
    bg_list->Data.change_run = 1;
    bg_list->Data.max_id = 0;
    bg_list->Data.exit_flag = 0;
    bg_list->Data.last_id = bg_list->Data.current_id;
    bg_list->Data.flash_run_time=0;
    bg_list->Data.flash_flag = FLASH_DISABLE;
    bg_list->Data.flash_time = FLASH_TIME;
    bg_list->Data.max_show_count = LCD_HEIGHT / 16 - 2;
    if (bg_list->Data.current_id <= bg_list->Data.max_show_count)
    {
        bg_list->Data.min_show_count = 0;
    }
    else
    {
        bg_list->Data.min_show_count = bg_list->Data.current_id - bg_list->Data.max_show_count;
    }


    return bg_list;
}

void BG_List_DeInit(BG_List* bg_list) {
    if (bg_list != NULL) {
        printf("DeInit\n"); // 如果BG_List结构体中包含需要释放的资源，在这里释放它们
        // 例如，如果Data结构体或head指针指向的链表需要释放，应该在这里做
        printf("DeInit\n"); printf("DeInit\n");
        // 释放BG_List结构体本身的内存
        free(bg_list);
    }
}