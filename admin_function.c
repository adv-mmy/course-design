#include"admin_function.h"

void displayInventoryStatus(InventoryManagement* inventory){
  ShelfNode* currentShelf=inventory->shelves;
  int cnt=0;
  while(currentShelf!=NULL){
    printf("第%d层共有包裹%d件，剩余空间：%fm\n", currentShelf->level, currentShelf->packageCnt, currentShelf->remainSpace);
    cnt+=currentShelf->packageCnt;
  }
  printf("仓库内共有%d件包裹\n", cnt);
  currentShelf=NULL;
  free(currentShelf);
  return;
}

void sentParcelRecording(PackageData** packageList){
  
}