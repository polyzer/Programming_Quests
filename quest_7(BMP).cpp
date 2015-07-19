#include <Windows.h>
#include <tchar.h>

#define IDM_EXIT 105

#define ID_INCREMENT_BRIGHT 1000
#define ID_REDUCE_BRIGHT 1001
#define ID_TO_BLACK_AND_WHITE 1002
#define ID_TURN_ON_90_DEGREES 1003
#define ID_PASTE_TOGETHER 1004
#define ID_FILE_OPEN 1005
#define ID_SAVE_BITMAP 1006

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int BitMapLoadAndSet(HBITMAP *hb, BITMAP *bm2, HDC *memBit2);
char *openFileDialog();
void ChangeBMP(HBITMAP *hBMP);
void pasteBitMapsTogether(HBITMAP *hBMP, BITMAP *BitMap, HDC *memBit, HBITMAP *hBMP2, BITMAP *BitMap2, HDC *memBit2);
void getBitMapsFromDevice(HBITMAP *hBMP, BITMAP *BitMap, HBITMAP *hBMP2, BITMAP *BitMap2, HDC *hdc, HDC *memBit, HDC *memBit2);

HINSTANCE hInst;
TCHAR WinName[] = _T("MainFrame") ;
HWND hWnd;              // Дескриптор главного окна программы

int APIENTRY _tWinMain(HINSTANCE This, // Дескриптор текущего приложения
	HINSTANCE Prev,                   // В современных системах всегда 0
	LPTSTR cmd,                       // Командная строка
	int mode)                         // Режим отображения окна
{
	HMENU hMainMenu; 
	HMENU hPopupMenuFile;
	HMENU hPopupMenuEdit;
	MSG msg;                // Структура для хранения сообщения
	WNDCLASS wc;  // Класс окна
// Определение класса окна
	wc.hInstance = This;
	wc.lpszClassName = WinName;                // Имя класса окна
	wc.lpfnWndProc = WndProc;                  // Функция окна
	wc.style = CS_HREDRAW | CS_VREDRAW;       // Стиль окна
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);    // Стандартная иконка
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Стандартный курсор
	wc.lpszMenuName = NULL;       // Нет меню
	wc.cbClsExtra = 0;            // Нет дополнительных данных класса
	wc.cbWndExtra = 0;            // Нет дополнительных данных окна
	// Заполнение окна белым цветом
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);
	if (!RegisterClass(&wc)) return 0;   // Регистрация класса окна
// Создание окна
	hWnd = CreateWindow(WinName, // Имя класса окна
    _T("Каркас Windows-приложения"),  // Заголовок окна
	WS_OVERLAPPEDWINDOW,         // Стиль окна
	CW_USEDEFAULT, // x
	CW_USEDEFAULT, // y   Размеры окна
	CW_USEDEFAULT, // width
	CW_USEDEFAULT, // Height
	HWND_DESKTOP, // Дескриптор родительского окна
	NULL,         // Нет меню
	This,         // Дескриптор приложения
	NULL);        // Дополнительной информации нет
	ShowWindow(hWnd, mode); // Показать окно
	hMainMenu= CreateMenu();
	hPopupMenuFile = CreatePopupMenu();
	hPopupMenuEdit = CreatePopupMenu();

    AppendMenu(hMainMenu, MF_STRING | MF_POPUP, (UINT) hPopupMenuFile, _T("&File")); 
    AppendMenu(hMainMenu, MF_STRING | MF_POPUP, (UINT) hPopupMenuEdit, _T("&Edit")); 
	 {
		 AppendMenu(hPopupMenuFile, MF_STRING, ID_FILE_OPEN, _T("Открыть"));
		 AppendMenu(hPopupMenuFile, MF_SEPARATOR, 1000, _T(""));
//		AppendMenu(hPopupMenuFile, MF_STRING, ID_SAVE_BITMAP, _T("Сохранить"));
//		AppendMenu(hPopupMenuFile, MF_SEPARATOR, 1000, _T(""));
		AppendMenu(hPopupMenuFile, MF_STRING, IDM_EXIT, _T("Выход"));
		AppendMenu(hPopupMenuFile, MF_SEPARATOR, 1000, _T(""));
     }
	 {
		AppendMenu(hPopupMenuEdit, MF_STRING, ID_INCREMENT_BRIGHT, _T("Увеличить яркость"));
		AppendMenu(hPopupMenuEdit, MF_STRING, ID_REDUCE_BRIGHT, _T("Уменьшить яркость"));
		AppendMenu(hPopupMenuEdit, MF_STRING, ID_TO_BLACK_AND_WHITE, _T("В черно-белое"));
		AppendMenu(hPopupMenuEdit, MF_STRING, ID_TURN_ON_90_DEGREES, _T("Поворот на 90"));
		AppendMenu(hPopupMenuEdit, MF_STRING, ID_PASTE_TOGETHER, _T("Склеить"));
     }

    SetMenu(hWnd, hMainMenu);


// Цикл обработки сообщений
	while(GetMessage(&msg, NULL, 0, 0))
	{
	TranslateMessage(&msg); // Функция трансляции кодов нажатой клавиши
	DispatchMessage(&msg);  // Посылает сообщение функции WndProc()
	}
	return 0;
}
// Оконная функция вызывается операционной системой
// и получает сообщения из очереди для данного приложения
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM
lParam)
{
	PAINTSTRUCT ps;
	static HDC hdc;
	static int TwoImages = 0;
	static int caption, menu, border;
	static HDC memBit, memBit2;
	static HBITMAP hBitmap, hBitmap2;
	static BITMAP bm, bm2;
	switch (message)
	{

	case WM_CREATE:
		caption = GetSystemMetrics(SM_CYCAPTION);
		menu = GetSystemMetrics (SM_CYMENU);
		border = GetSystemMetrics(SM_CXFIXEDFRAME);
		hBitmap = (HBITMAP)LoadImage(NULL, _T("db.bmp"), IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		if (hBitmap == NULL)
		{
			MessageBox(hWnd,_T("Файл не найден"),_T("Загрузка изображения"),
			MB_OK | MB_ICONHAND);
			DestroyWindow(hWnd);
			return 1;
		}
		GetObject(hBitmap, sizeof(bm), &bm);
		hdc = GetDC(hWnd);
		memBit = CreateCompatibleDC(hdc);
		memBit2 = CreateCompatibleDC(hdc);
		SelectObject(memBit, hBitmap);
		ReleaseDC(hWnd, hdc);
	break;

	case WM_SIZE:		
		if (TwoImages > 0)
			if (bm.bmHeight > bm2.bmHeight)
				MoveWindow(hWnd,100, 50, (bm.bmWidth+2 + bm2.bmWidth) *border, bm.bmHeight + caption
				+ menu + border, FALSE);
			else
				MoveWindow(hWnd,100, 50, (bm.bmWidth+2 + bm2.bmWidth) *border, bm2.bmHeight + caption
				+ menu + border, FALSE);
		else
			MoveWindow(hWnd,100, 50, bm.bmWidth+2 *border, bm.bmHeight + caption
			+ menu + border, FALSE);

	break;

	case WM_COMMAND:
	switch (LOWORD(wParam))
	{
		case ID_INCREMENT_BRIGHT:
			ChangeBMP(&hBitmap, &bm, &memBit, 1);
			InvalidateRect(hWnd, NULL, TRUE);
		break;
		case ID_REDUCE_BRIGHT:
			ChangeBMP(&hBitmap, &bm, &memBit, 2);
			InvalidateRect(hWnd, NULL, TRUE);
		break;
		case ID_TO_BLACK_AND_WHITE:
			ChangeBMP(&hBitmap, &bm, &memBit, 3);
			InvalidateRect(hWnd, NULL, TRUE);
		break;
		case ID_TURN_ON_90_DEGREES:
			ChangeBMP(&hBitmap, &bm, &memBit, 4);
			InvalidateRect(hWnd, NULL, TRUE);
		break;
		case ID_PASTE_TOGETHER:
			if (TwoImages > 0){
				pasteBitMapsTogether(&hBitmap, &bm, &memBit, &hBitmap2, &bm2, &memBit2);
				//getBitMapsFromDevice(&hBitmap, &bm, &hBitmap2, &bm2, &hdc, &memBit, &memBit2);
				TwoImages = 0;
				InvalidateRect(hWnd, NULL, TRUE);
			}
		break;
		case ID_FILE_OPEN:
			BitMapLoadAndSet(&hBitmap2, &bm2, &memBit2); 
			TwoImages = 1;
			//SendMessage(hWnd, WM_SIZE, 0,0);
			InvalidateRect(hWnd, NULL, TRUE);
		break;
		case ID_SAVE_BITMAP:
			
		break;
		case IDM_EXIT: 
			DestroyWindow(hWnd); 
		break;
	
		default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	break;
	
	case WM_PAINT:
		if (TwoImages > 0)
			if (bm.bmHeight > bm2.bmHeight)
				MoveWindow(hWnd,100, 50, (bm.bmWidth+2 + bm2.bmWidth) *border, bm.bmHeight + caption
				+ menu + border, FALSE);
			else
				MoveWindow(hWnd,100, 50, (bm.bmWidth+2 + bm2.bmWidth) *border, bm2.bmHeight + caption
				+ menu + border, FALSE);
		else
			MoveWindow(hWnd,100, 50, bm.bmWidth+2 *border, bm.bmHeight + caption
			+ menu + border, FALSE);

		hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, memBit, 0, 0, SRCCOPY);
		if (TwoImages > 0)
			BitBlt(hdc, bm.bmWidth, 0, bm2.bmWidth, bm2.bmHeight, memBit2, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		
	break;

	case WM_DESTROY: 
		DeleteDC(memBit);
		DeleteDC(memBit2);
		PostQuitMessage(0); 
	break;
	
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
return 0;
}

int SaveBitMap(){
	

}

int BitMapLoadAndSet(HBITMAP *hb, BITMAP *bm2, HDC *memBit2)
{
	*hb = (HBITMAP)LoadImage(NULL, (LPCSTR) openFileDialog(), IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (*hb == NULL)
	{
		MessageBox(hWnd,_T("Файл не найден"),_T("Загрузка изображения"),
		MB_OK | MB_ICONHAND);
		DestroyWindow(hWnd);
		return 1;
	}
	GetObject(*hb, sizeof(*bm2), bm2);
	SelectObject(*memBit2, *hb);
	return 0;
}


char *openFileDialog(){
	OPENFILENAME ofn={0};
    char szDirect[260];
	char *szFileName = (char*) malloc(MAX_PATH * sizeof(char));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFileName;
    *(ofn.lpstrFile) = 0;
    ofn.nMaxFile =sizeof(szDirect);
    ofn.lpstrFilter = (LPCSTR) "BITMAP Images (*.bmp)\0*.bmp\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = szDirect;
    *(ofn.lpstrFileTitle) = 0;
    ofn.nMaxFileTitle = sizeof(szFileName);
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_EXPLORER;
    GetOpenFileName(&ofn);
	return szFileName;

}

void getBitMapsFromDevice(HBITMAP *hBMP, BITMAP *BitMap, HBITMAP *hBMP2, BITMAP *BitMap2, HDC *hdc, HDC *memBit, HDC *memBit2)
{
	int temp = 0, i = 0, j = 0, k = 0, shift = 0, R = 0, G = 0, B = 0, height = 0, width = 0;
	if (BitMap->bmHeight > BitMap2->bmHeight){
		height = BitMap->bmHeight;
	}
	else{
		height = BitMap2->bmHeight;
	}
//	BitBlt(*memBit, BitMap->bmWidth, 0, BitMap2->bmWidth,BitMap2->bmHeight, *memBit2, 0, 0, SRCCOPY);
	//BitBlt(*hdc, BitMap->bmWidth, 0, BitMap2->bmWidth,BitMap2->bmHeight, *memBit2, 0, 0, SRCCOPY);
   width = BitMap->bmWidth + BitMap2->bmWidth;
   BitBlt(*memBit, 0, 0, height, width, *hdc, 0, 0, SRCCOPY);
   *hBMP = CreateCompatibleBitmap(*memBit, width, height);
//   SelectObject(*memBit, *hBMP);
   GetObject(*hBMP, sizeof(BITMAP), BitMap);
 
}


void ChangeBMP(HBITMAP *hBMP, BITMAP *BitMap, HDC *memBit, int type) 
{
  //types:
  //1 - reduce brightness
  //2 - increase brightness
  //3 - to black-white image
  //4 - turning 90
  //5 - paste 2 images together
  int temp = 0, shift = 0,  i = 0, j = 0, k = 0, incr = 10, decr = 10,R = 0, G = 0, B = 0,  PixelBytesSize = 0;
  BYTE *tempLine, *Lines;
  struct tagBITMAPINFOHEADER bInfo;
  {
	  bInfo.biSize = sizeof(bInfo);
	  bInfo.biWidth = BitMap->bmWidth;  
	  bInfo.biHeight = BitMap->bmHeight; 
	  bInfo.biBitCount = BitMap->bmBitsPixel;  //это количество бит на пиксель
	  bInfo.biPlanes =  BitMap->bmPlanes;
	  bInfo.biCompression = BI_RGB;

  }
  PixelBytesSize = bInfo.biBitCount / 8;
  switch(type)
  {
   case 1:
   tempLine = (BYTE *) malloc(bInfo.biWidth * (bInfo.biBitCount/8) * sizeof(BYTE));
   for (i = 0; i < (bInfo.biHeight); i++) 
   {
		GetDIBits(*memBit, *hBMP, i, 1, tempLine,(LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
		for(j = 0; j < bInfo.biWidth * PixelBytesSize; j+= PixelBytesSize) 
		{
			if(tempLine[j] < (255 - incr) &&
			   tempLine[j + 1] < (255 - incr) &&
			   tempLine[j + 2] < (255 - incr))
			{
				if (tempLine[j] < (255 - incr))
					tempLine[j] += incr;
				if (tempLine[j + 1] < (255 - incr))
					tempLine[j + 1] += incr;
				if (tempLine[j + 2] < (255 - incr))
					tempLine[j + 2] += incr;
			}
		}			
		SetDIBits(*memBit, *hBMP, i, 1, tempLine, (LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
   }
	free (tempLine);
   break;
   case 2:
   tempLine = (BYTE *) malloc(bInfo.biWidth * PixelBytesSize * sizeof(BYTE));
   for (i = 0; i < (bInfo.biHeight); i++) 
   {
		GetDIBits(*memBit, *hBMP, i, 1, tempLine,(LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
		for(j = 0; j < bInfo.biWidth * PixelBytesSize; j+= PixelBytesSize) 
		{
			if(tempLine[j] > (0 + incr) &&
			   tempLine[j + 1] > (0 + incr) &&
			   tempLine[j + 2] > (0 + incr))
			{
				if (tempLine[j] > (0 + incr))
					tempLine[j] -= incr;
				if (tempLine[j + 1] > (0 + incr))
					tempLine[j + 1] -= incr;
				if (tempLine[j + 2] > (0 + incr))
					tempLine[j + 2] -= incr;
			}
		}			
		SetDIBits(*memBit, *hBMP, i, 1, tempLine, (LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
   }
	free (tempLine); 
  break;
  case 3:
   tempLine = (BYTE *) malloc(bInfo.biWidth * (PixelBytesSize) * sizeof(BYTE));
   for (i = 0; i < (bInfo.biHeight); i++) 
   {
		GetDIBits(*memBit, *hBMP, i, 1, tempLine,(LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
		for(j = 0; j < bInfo.biWidth * PixelBytesSize; j+= PixelBytesSize) 
		{
			if((tempLine[j] +
			   tempLine[j + 1] +
			   tempLine[j + 2]) < (765 / 2))
			{
					tempLine[j] = 0;
					tempLine[j + 1]  = 0;
					tempLine[j + 2] = 0;
			} else {
					tempLine[j] = 255;
					tempLine[j + 1]  = 255;
					tempLine[j + 2] = 255;			
			}
		}			
		SetDIBits(*memBit, *hBMP, i, 1, tempLine, (LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
   }
	free (tempLine);
  break;
  //переворот изображения на 90
  case 4:
	  tempLine = (BYTE *) malloc(bInfo.biWidth * PixelBytesSize * sizeof(BYTE));
	  Lines = (BYTE *) malloc(bInfo.biHeight * bInfo.biWidth * PixelBytesSize * sizeof(BYTE));
	  for (i = 0; i < bInfo.biHeight; i++) {
		  GetDIBits(*memBit, *hBMP, i, 1, tempLine,(LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
		  k = i * PixelBytesSize;//shift вначале = 0	
		  for(j = (bInfo.biWidth - 1) * PixelBytesSize; j >= 0; j-= PixelBytesSize) {
			Lines[k] = tempLine[j];
			Lines[k + 1] = tempLine[j + 1];
			Lines[k + 2] = tempLine[j + 2];
			k += bInfo.biHeight * PixelBytesSize;			
		  }   
	  }
	  temp = bInfo.biWidth;//переставляем ширину и длину местами
	  
	  bInfo.biWidth = bInfo.biHeight;
	  bInfo.biHeight = temp;
	  BitMap->bmHeight = bInfo.biHeight;
	  BitMap->bmWidth = bInfo.biWidth;
	  BitMap->bmWidthBytes = BitMap->bmWidth;
	  BitMap->bmBits = (void *) Lines;
	  
	  SetDIBitsToDevice(*memBit, 0, 0, bInfo.biWidth, bInfo.biHeight,0, 0,0,bInfo.biHeight, BitMap->bmBits, (LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
	  *hBMP = CreateCompatibleBitmap(*memBit, BitMap->bmWidth, BitMap->bmHeight);
	  SetDIBits(*memBit, *hBMP, 0, BitMap->bmHeight, BitMap->bmBits,(LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
	  SelectObject(*memBit, *hBMP);
	  Lines = NULL;
	  free(tempLine);
  break;
  }


}

void pasteBitMapsTogether(HBITMAP *hBMP, BITMAP *BitMap, HDC *memBit, HBITMAP *hBMP2, BITMAP *BitMap2, HDC *memBit2)
{
	int temp = 0, i = 0, j = 0, k = 0, height = 0, width = 0, PixelBytesSize = 0, PixelBytesSize2 = 0;
	short int heigher;
    BYTE *tempLine1, *tempLine2, *Lines;
    struct tagBITMAPINFOHEADER bInfo, bInfo2;
    {
	    bInfo.biSize = sizeof(bInfo);
	    bInfo.biWidth = BitMap->bmWidth;  
	    bInfo.biHeight = BitMap->bmHeight; 
		bInfo.biBitCount = BitMap->bmBitsPixel;  //это количество бит на пиксель
		bInfo.biPlanes = BitMap->bmPlanes;
	    bInfo.biCompression = BI_RGB;
    }
	{
	    bInfo2.biSize = sizeof(bInfo2);
	    bInfo2.biWidth = BitMap2->bmWidth;  
	    bInfo2.biHeight = BitMap2->bmHeight; 
		bInfo2.biBitCount = BitMap2->bmBitsPixel;  //это количество бит на пиксель
	    bInfo2.biPlanes = BitMap2->bmPlanes;
	    bInfo2.biCompression = BI_RGB;
    }
	if (bInfo.biHeight > bInfo2.biHeight){
		height = bInfo.biHeight;
	}
	else{
		height = bInfo2.biHeight;
	}
	PixelBytesSize = bInfo.biBitCount / 8;
	PixelBytesSize2 = bInfo2.biBitCount / 8;

   width = bInfo.biWidth + bInfo2.biWidth;
   Lines = (BYTE *) malloc((bInfo.biWidth + bInfo2.biWidth) * height * 3 * sizeof(BYTE));
   tempLine1 = (BYTE *) malloc(bInfo.biWidth * PixelBytesSize * sizeof(BYTE));
   tempLine2 = (BYTE *) malloc(bInfo2.biWidth * PixelBytesSize2 * sizeof(BYTE));
   
   for (i = 0; i < (height); i++) 
   {
	   if(i < bInfo.biHeight)
			GetDIBits(*memBit, *hBMP, i, 1, tempLine1,(LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
	   if(i < bInfo2.biHeight)
			GetDIBits(*memBit2, *hBMP2, i, 1, tempLine2,(LPBITMAPINFO) &bInfo2, DIB_RGB_COLORS);
		for(j = 0; j < bInfo.biWidth * PixelBytesSize; j+= PixelBytesSize) 
		{
			if(i < bInfo.biHeight)
			{
				Lines[j + k] = tempLine1[j];
				Lines[j + k + 1] = tempLine1[j + 1];
				Lines[j + k + 2] = tempLine1[j + 2];
			}else
			{
				Lines[j + k] = 0;
				Lines[j + k + 1] = 0;
				Lines[j + k + 2] = 0;
		    } 
		}
		for(j = 0; j < bInfo2.biWidth * PixelBytesSize2; j += PixelBytesSize2) 
		{
			if(i < bInfo2.biHeight)
			{
				Lines[bInfo.biWidth * PixelBytesSize + j + k] = tempLine2[j];
				Lines[bInfo.biWidth * PixelBytesSize + j + k + 1] = tempLine2[j + 1];
				Lines[bInfo.biWidth * PixelBytesSize + j + k + 2] = tempLine2[j + 2];
			}else
			{
				Lines[bInfo.biWidth * PixelBytesSize + j + k] = 0;
				Lines[bInfo.biWidth * PixelBytesSize + j + k + 1] = 0;
				Lines[bInfo.biWidth * PixelBytesSize + j + k + 2] = 0;
		   } 
		}
		k += bInfo.biWidth * PixelBytesSize + bInfo2.biWidth * PixelBytesSize2; //смещение
   }
	  bInfo.biWidth = width;
	  bInfo.biHeight = height;
	  BitMap->bmHeight = bInfo.biHeight;
	  BitMap->bmWidth = bInfo.biWidth;
	  BitMap->bmWidthBytes = BitMap->bmWidth + BitMap2->bmWidth;
	  BitMap->bmBits = (void *) Lines; 
	  SetDIBitsToDevice(*memBit, 0, 0, bInfo.biWidth, bInfo.biHeight, 0, 0, 0, bInfo.biHeight, BitMap->bmBits, (LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
	  *hBMP = CreateCompatibleBitmap(*memBit, BitMap->bmWidth, BitMap->bmHeight);
	  SetDIBits(*memBit, *hBMP, 0, BitMap->bmHeight, BitMap->bmBits,(LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
	  SelectObject(*memBit, *hBMP);
	  Lines = NULL;
      free(tempLine1);
	  free(tempLine2);


}