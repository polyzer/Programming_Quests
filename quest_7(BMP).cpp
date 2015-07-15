#include <Windows.h>
#include <tchar.h>
#define IDM_EXIT 105
#define ID_INCREMENT_BRIGHT 1000
#define ID_REDUCE_BRIGHT 1001
#define ID_TO_BLACK_AND_WHITE 1002
#define ID_TURN_ON_90_DEGREES 1003
#define ID_PASTE_TOGETHER 1004

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

void ChangeBMP(HBITMAP *hBMP);

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
		 AppendMenu(hPopupMenuFile, MF_STRING, 1000, _T("Открыть"));
		 AppendMenu(hPopupMenuFile, MF_SEPARATOR, 1000, _T(""));
     AppendMenu(hPopupMenuFile, MF_STRING, 1000, _T("Сохранить"));
     AppendMenu(hPopupMenuFile, MF_SEPARATOR, 1000, _T(""));
     }
	 {
     AppendMenu(hPopupMenuEdit, MF_STRING, ID_INCREMENT_BRIGHT, _T("Увеличить яркость"));
     AppendMenu(hPopupMenuEdit, MF_STRING, ID_REDUCE_BRIGHT, _T("Уменьшить яркость"));
     AppendMenu(hPopupMenuEdit, MF_STRING, ID_TO_BLACK_AND_WHITE, _T("В черно-белое"));
     AppendMenu(hPopupMenuEdit, MF_STRING, 1000, _T("Отразить по горизонтали"));
     AppendMenu(hPopupMenuEdit, MF_STRING, 1000, _T("Отразить по вертикали"));
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
	HDC hdc;
	static int caption, menu, border;
	static HDC memBit;
	static HBITMAP hBitmap;
	static BITMAP bm;
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
		SelectObject(memBit, hBitmap);
		ReleaseDC(hWnd, hdc);
	break;

	case WM_SIZE:
		MoveWindow(hWnd,100, 50, bm.bmWidth+2 *border, bm.bmHeight + caption
		+ menu + border, TRUE);
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
			ChangeBMP(&hBitmap, &bm, &memBit, 5);
			InvalidateRect(hWnd, NULL, TRUE);
		break;

		case IDM_EXIT: 
			DestroyWindow(hWnd); 
		break;
	
		default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	break;
	
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, memBit, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
	break;

	case WM_DESTROY: PostQuitMessage(0); break;
	
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
return 0;
}

void ChangeBMP(HBITMAP *hBMP, BITMAP *BitMap, HDC *memBit, int type) 
{
  //types:
  //1 - reduce brightness
  //2 - increase brightness
  //3 - to black-white image
  //4 - turning 90
  //5 - paste 2 images together
  int temp = 0, shift = 0,  i = 0, j = 0, k = 0, incr = 10, decr = 10,R = 0, G = 0, B = 0;
  BYTE *tempLine, *tempScanLine1, *Lines;
  struct tagBITMAPINFOHEADER bInfo;
  {
	  bInfo.biSize = sizeof(bInfo);
	  bInfo.biWidth = BitMap->bmWidth;  
	  bInfo.biHeight = BitMap->bmHeight; 
	  bInfo.biBitCount = 24;  //это количество бит на пиксель
	  bInfo.biPlanes = 1;
	  bInfo.biCompression = BI_RGB;
  }
  switch(type)
  {
   case 1:
   tempScanLine1 = (BYTE *) malloc(bInfo.biWidth * (bInfo.biBitCount/8) * sizeof(BYTE));
   for (i = 0; i < (bInfo.biHeight); i++) 
   {
		GetDIBits(*memBit, *hBMP, i, 1, tempScanLine1,(LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
		for(j = 0; j < bInfo.biWidth * 3; j+= 3) 
		{
			if(tempScanLine1[j] < (255 - incr) &&
			   tempScanLine1[j + 1] < (255 - incr) &&
			   tempScanLine1[j + 2] < (255 - incr))
			{
				if (tempScanLine1[j] < (255 - incr))
					tempScanLine1[j] += incr;
				if (tempScanLine1[j + 1] < (255 - incr))
					tempScanLine1[j + 1] += incr;
				if (tempScanLine1[j + 2] < (255 - incr))
					tempScanLine1[j + 2] += incr;
			}
		}			
		SetDIBits(*memBit, *hBMP, i, 1, tempScanLine1, (LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
   }
	free (tempScanLine1);
   break;
   case 2:
   tempScanLine1 = (BYTE *) malloc(bInfo.biWidth * (bInfo.biBitCount/8) * sizeof(BYTE));
   for (i = 0; i < (bInfo.biHeight); i++) 
   {
		GetDIBits(*memBit, *hBMP, i, 1, tempScanLine1,(LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
		for(j = 0; j < bInfo.biWidth * 3; j+= 3) 
		{
			if(tempScanLine1[j] > (0 + incr) &&
			   tempScanLine1[j + 1] > (0 + incr) &&
			   tempScanLine1[j + 2] > (0 + incr))
			{
				if (tempScanLine1[j] > (0 + incr))
					tempScanLine1[j] -= incr;
				if (tempScanLine1[j + 1] > (0 + incr))
					tempScanLine1[j + 1] -= incr;
				if (tempScanLine1[j + 2] > (0 + incr))
					tempScanLine1[j + 2] -= incr;
			}
		}			
		SetDIBits(*memBit, *hBMP, i, 1, tempScanLine1, (LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
   }
	free (tempScanLine1); 
  break;
  case 3:
   tempScanLine1 = (BYTE *) malloc(bInfo.biWidth * (bInfo.biBitCount/8) * sizeof(BYTE));
   for (i = 0; i < (bInfo.biHeight); i++) 
   {
		GetDIBits(*memBit, *hBMP, i, 1, tempScanLine1,(LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
		for(j = 0; j < bInfo.biWidth * 3; j+= 3) 
		{
			if((tempScanLine1[j] +
			   tempScanLine1[j + 1] +
			   tempScanLine1[j + 2]) < (765 / 2))
			{
					tempScanLine1[j] = 0;
					tempScanLine1[j + 1]  = 0;
					tempScanLine1[j + 2] = 0;
			} else {
					tempScanLine1[j] = 255;
					tempScanLine1[j + 1]  = 255;
					tempScanLine1[j + 2] = 255;			
			}
		}			
		SetDIBits(*memBit, *hBMP, i, 1, tempScanLine1, (LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
   }
	free (tempScanLine1);
  break;
  case 4:
	  tempLine = (BYTE *) malloc(bInfo.biWidth * (bInfo.biBitCount / 8) * sizeof(BYTE));
	  Lines = (BYTE *) malloc(bInfo.biHeight * bInfo.biWidth * (bInfo.biBitCount / 8) * sizeof(BYTE));
	  for (i = 0; i < bInfo.biHeight; i++) {
		  GetDIBits(*memBit, *hBMP, i, 1, tempLine,(LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
		  k = i * (bInfo.biBitCount / 8);//shift вначале = 0	
		  for(j = (bInfo.biWidth - 1) * (bInfo.biBitCount / 8); j >= 0; j-= 3) {
			Lines[k] = tempLine[j];
			Lines[k + 1] = tempLine[j + 1];
			Lines[k + 2] = tempLine[j + 2];
			k += bInfo.biHeight * (bInfo.biBitCount / 8);			
		  } 
		shift = 0;  
	  }
	  temp = bInfo.biWidth;//переставляем ширину и длину местами
	  bInfo.biWidth = bInfo.biHeight;
	  bInfo.biHeight = temp;
	  BitMap->bmHeight = bInfo.biHeight;
	  BitMap->bmWidth = bInfo.biWidth;
	  BitMap->bmWidthBytes = BitMap->bmWidth;
	  BitMap->bmBits = (void *) Lines;
	  *hBMP = CreateBitmapIndirect(BitMap);
	  SetDIBits(*memBit, *hBMP, 0, BitMap->bmHeight, BitMap->bmBits,(LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
	  SetDIBitsToDevice(*memBit, 0, 0, bInfo.biWidth, bInfo.biHeight,0, 0,0,  bInfo.biHeight, BitMap->bmBits, (LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
	  *hBMP = CreateCompatibleBitmap(*memBit, BitMap->bmWidth, BitMap->bmHeight);
	  SetDIBits(*memBit, *hBMP, 0, BitMap->bmHeight, BitMap->bmBits,(LPBITMAPINFO) &bInfo, DIB_RGB_COLORS);
	  Lines = NULL;
	free(tempLine);
  break;
  case 5:
	break;  
  }


}

