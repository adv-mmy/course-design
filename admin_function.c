#include"admin_function.h"

void displayInventoryStatus(InventoryManagement* inventory){
  ShelfNode* currentShelf=inventory->shelves;
  while(currentShelf!=NULL){
    printf("第%d层剩余空间：%fm\n", currentShelf->level, currentShelf->remainSpace);
  }
  currentShelf=NULL;
  free(currentShelf);
  return;
}

void sentParcelRecording(PackageData* packageList){
  
}