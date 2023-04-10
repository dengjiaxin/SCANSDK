// ESL_SAMPLE.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ESL_SAMPLE.h"
#include "EslStub.h"

#define LICENSE_KEY	"EPSON"
#define	DEVICE_NAME	TEXT("DS-970")

BOOL CALLBACK eslCallback(EslCBDataHdr* pData, PVOID pPrivate)
{
	// Check if the parameter is valid
	if (pData == NULL) {
		ES_TRACE(TEXT("[ERROR]pData is NULL\n"));
		return FALSE;
	}

	ES_TRACE0(TEXT("========ESL CALLBACK============\n"));
	switch (pData->type) {
		case CBDT_STATUS_SCAN:
		{
			ES_TRACE(TEXT("[INFO]CBDT_STATUS_SCAN\n"));
			EslCBDataScanStatus* pstatus = (EslCBDataScanStatus*)pData;
			switch (pstatus->status) {
			case STATUS_SCAN_UPDATE_PROGRESS:
				ES_TRACE(TEXT("[INFO]STATUS_SCAN_UPDATE_PROGRESS\n"));
				ES_TRACE1(TEXT("[INFO]Count:%d\n"), pstatus->count);
				ES_TRACE1(TEXT("[INFO]Progress:%d\n"), pstatus->progress);
				break;
			case STATUS_SCAN_PAGE_END:
				ES_TRACE(TEXT("[INFO]STATUS_SCAN_PAGE_END\n"));
				ES_TRACE1(TEXT("[INFO]Count:%d\n"), pstatus->count);
				break;
			default:
				ES_TRACE1(TEXT("[ERROR]Invalid CallbackData Status(%x)\n"), pstatus->status);
			}

			// If application wants to cancel scanning, return FALSE
			BOOL cancelReqfromApp = FALSE;
			if (cancelReqfromApp) {
				return FALSE;
			}

			break;
		}
		case CBDT_IMAGE_FILE:
		{
			ES_TRACE(TEXT("[INFO]CBDT_IMAGE_FILE\n"));
			// Get saved image file information
			EslCBDImageFile* pstatus = (EslCBDImageFile*)pData;
			switch (pstatus->status) {
			case STATUS_IMAGE_FILE_INFO:
				ES_TRACE(TEXT("[INFO]STATUS_IMAGE_FILE_INFO\n"));
				ES_TRACE1(TEXT("[INFO]Filename:%s\n"), pstatus->filename);
				break;
			default:
				ES_TRACE1(TEXT("[ERROR]Invalid CallbackData Status(%x)\n"), pstatus->status);
			}
			break;
		}
		case CBDT_IMAGE_MEMORY:
		{
			ES_TRACE(TEXT("[INFO]CBDT_IMAGE_MEMORY\n"));
			// Get image data from library
			EslCBDataImageMemory* pstatus = (EslCBDataImageMemory*)pData;
			switch (pstatus->status) {
			case STATUS_IMAGE_MEMORY_INFO:
				{
					ES_TRACE(TEXT("[INFO]STATUS_IMAGE_MEMORY_INFO\n"));
					ES_TRACE1(TEXT("[INFO]count:%d\n"), pstatus->count);
					ES_TRACE1(TEXT("[INFO]img_width:%d\n"), pstatus->img_width);
					ES_TRACE1(TEXT("[INFO]img_height:%d\n"), pstatus->img_height);
					ES_TRACE1(TEXT("[INFO]img_resolution:%d\n"), pstatus->img_resolution);
					ES_TRACE1(TEXT("[INFO]img_channel_per_pixel:%d\n"), pstatus->img_channel_per_pixel);
					ES_TRACE1(TEXT("[INFO]img_bits_per_channel:%d\n"), pstatus->img_bits_per_channel);
					ES_TRACE1(TEXT("[INFO]img_data_size:%d\n"), pstatus->img_data_size);
					ES_TRACE1(TEXT("[INFO]img_handle:%p\n"), pstatus->img_handle);
					CString filename;
					filename.Format(TEXT("C:\\SAMPLE%04d.BMP"), pstatus->count);
					debugSave(pstatus->img_handle, filename);
					break;
				}
			default:
				ES_TRACE1(TEXT("[ERROR]Invalid CallbackData Status(%x)\n"), pstatus->status);
			}
			break;
		}
		case CBDT_DATA_OBR:
		{
			ES_TRACE(TEXT("[INFO]CBDT_DATA_OBR\n"));
			// Get image data from library
			EslCBDataOBRData* pstatus = (EslCBDataOBRData*)pData;
			switch (pstatus->status) {
			case STATUS_DATA_OBR_RESULT:
				ES_TRACE(TEXT("[INFO]STATUS_DATA_OBR_RESULT\n"));
				ES_TRACE1(TEXT("[INFO]count:%d\n"), pstatus->count);
				ES_TRACE1(TEXT("[INFO]index:%d\n"), pstatus->index);
				ES_TRACE1(TEXT("[INFO]symbolType:%x\n"), pstatus->symbolType);
				ES_TRACE1(TEXT("[INFO]dataLength:%d\n"), pstatus->dataLength);
				ES_TRACE1(TEXT("[INFO]dataString:%s\n"), pstatus->dataString);
				ES_TRACE1(TEXT("[INFO]orientation:%d\n"), pstatus->orientation);
				break;
			default:
				ES_TRACE1(TEXT("[ERROR]Invalid CallbackData Status(%x)\n"), pstatus->status);
			}
			break;
		}
		case CBDT_DATA_OCR:
		{
			ES_TRACE(TEXT("[INFO]CBDT_DATA_OCR\n"));
			// Get image data from library
			EslCBDataOCRData* pstatus = (EslCBDataOCRData*)pData;
			switch (pstatus->status) {
			case STATUS_DATA_OCR_RESULT:
				ES_TRACE(TEXT("[INFO]STATUS_DATA_OCR_RESULT\n"));
				ES_TRACE1(TEXT("[INFO]count:%d\n"), pstatus->count);
				ES_TRACE1(TEXT("[INFO]dataLength:%d\n"), pstatus->dataLength);
				ES_TRACE1(TEXT("[INFO]dataString:%s\n"), pstatus->dataString);
				break;
			default:
				ES_TRACE1(TEXT("[ERROR]Invalid CallbackData Status(%x)\n"), pstatus->status);
			}
			break;
		}
		default:
			ES_TRACE1(TEXT("[ERROR]Invalid CallbackData Type(%x)\n"), pData->type);
	}
	ES_TRACE0(TEXT("================================\n"));

	return TRUE;
}

DWORD SCAN_SAVE_Default(void)
{
	DWORD instance = 0;
	DWORD err = 0;

	CEslStub eslib(NULL);

	//
	// Initialize Library
	//
	BOOL ret = eslib.eslInit(&instance, LICENSE_KEY, &err);
	if (ret) {

		//
		// Open Scanner
		//
		ConnectInfo scanner = {CT_DEFAULT, DEVICE_NAME}; // specify connection type and device name
		ret = eslib.eslOpenScanner(instance, &scanner, &err);
		if (ret) {
			if (ret) {
				// Scan start(Scan and Save)
				ret = eslib.eslExecute(instance, ET_SCAN_AND_SAVE, eslCallback, NULL, &err);
			}

			// Close scanner
			ret = eslib.eslCloseScanner(instance, &err);
		}

		// Free Library
		eslib.eslFree(instance, &err);
	}

	return err;
}

DWORD SCAN_SAVE_with_OBRFileName(void)
{
	DWORD instance = 0;
	DWORD err = 0;

	CEslStub eslib(NULL);

	//
	// Initialize Library
	//
	BOOL ret = eslib.eslInit(&instance, LICENSE_KEY, &err);
	if (ret) {

		//
		// Open Scanner
		//
		ConnectInfo scanner = {CT_DEFAULT, DEVICE_NAME}; // specify connection type and device name
		ret = eslib.eslOpenScanner(instance, &scanner, &err);
		if (ret) {
			//
			// Update Save Basic parameters
			//
			EslParamSaveExV1 save_params;
			save_params.type = ESL_PARAM_SAVE_EX_V1;
			save_params.size = sizeof(save_params);

			// Get default parameters
			if (ret) {
				ret = eslib.eslCtrlParameter(instance, POT_GET_DEFAULT, (pEslParamHdr)&save_params, &err);
			}

			// Change some parameters...
			if (ret) {
				save_params.fileFormat = FF_JPEG;					// File format is JPEG
				StringCbCopy(save_params.filePath, sizeof(save_params.filePath), TEXT("C:\\TEST"));						// File save path
				StringCbCopy(save_params.fileNamePrefix, sizeof(save_params.fileNamePrefix), TEXT("barcode_filename"));	// Prefix string for file name
				save_params.fileNameType = FN_BARCODE;				// File name uses Barcode data
				save_params.fileNumCounterDigits = 4;				// 4 digit file name counter is added 
				save_params.fileNumStartCount = 1;					// File name counter start from 1
				save_params.fileBCType = 0xFF;						// Accept all Barcode type

				// Set parameters
				ret = eslib.eslCtrlParameter(instance, POT_SET, (pEslParamHdr)&save_params, &err);
			}

			if (ret) {
				// Scan start(Scan and Save)
				ret = eslib.eslExecute(instance, ET_SCAN_AND_SAVE, eslCallback, NULL, &err);
			}

			// Close scanner
			ret = eslib.eslCloseScanner(instance, &err);
		}

		// Free Library
		eslib.eslFree(instance, &err);
	}

	return err;
}

DWORD SCAN_SAVE_with_OBRFolderName(void)
{
	DWORD instance = 0;
	DWORD err = 0;

	CEslStub eslib(NULL);

			//
	// Initialize Library
			//
	BOOL ret = eslib.eslInit(&instance, LICENSE_KEY, &err);
	if (ret) {

		//
		// Open Scanner
		//
		ConnectInfo scanner = {CT_DEFAULT, DEVICE_NAME}; // specify connection type and device name
		ret = eslib.eslOpenScanner(instance, &scanner, &err);
		if (ret) {
			//
			// Update Save Basic parameters
			//
			EslParamSaveExV1 save_params;
			save_params.type = ESL_PARAM_SAVE_EX_V1;
			save_params.size = sizeof(save_params);

			// Get default parameters
			if (ret) {
				ret = eslib.eslCtrlParameter(instance, POT_GET_DEFAULT, (pEslParamHdr)&save_params, &err);
			}

			// Change some parameters...
			if (ret) {
				save_params.fileFormat = FF_JPEG;					// File format is JPEG
				StringCbCopy(save_params.filePath, sizeof(save_params.filePath), TEXT("C:\\TEST"));							// File save path
				StringCbCopy(save_params.fileNamePrefix, sizeof(save_params.fileNamePrefix), TEXT("barcode_separated"));	// Prefix string for file name
				save_params.fileNameType = FN_PREFIX;				// File name uses prefix
				save_params.fileNumCounterDigits = 4;				// 4 digit file name counter is added 
				save_params.fileNumStartCount = 1;					// File name counter start from 1
				save_params.assortType = AT_BARCODE;				// Separate folder with Barcode data
				save_params.assortOption = ESL_REMOVE_SEPARATOR;	// Discard Barcode page as only separater usage
				save_params.assortBCType = 0xFF;					// Accept all Barcode type
				save_params.folderNameType = FON_BARCODE_OCR;		// Folder name uses Barcode data

				// Set parameters
				ret = eslib.eslCtrlParameter(instance, POT_SET, (pEslParamHdr)&save_params, &err);
			}

			if (ret) {
				// Scan start(Scan and Save)
				ret = eslib.eslExecute(instance, ET_SCAN_AND_SAVE, eslCallback, NULL, &err);
			}

			// Close scanner
			ret = eslib.eslCloseScanner(instance, &err);
		}

		// Free Library
		eslib.eslFree(instance, &err);
	}

	return err;
}

DWORD SCAN_STORE_SAVE_Default(void)
{
	DWORD instance = 0;
	DWORD err = 0;

	CEslStub eslib(NULL);

	//
	// Initialize Library
	//
	BOOL ret = eslib.eslInit(&instance, LICENSE_KEY, &err);
	if (ret) {

		//
		// Open Scanner
		//
		ConnectInfo scanner = {CT_DEFAULT, DEVICE_NAME}; // specify connection type and device name
		ret = eslib.eslOpenScanner(instance, &scanner, &err);
		if (ret) {

			if (ret) {
				// Scan start(Scan and Store)
				ret = eslib.eslExecute(instance, ET_SCAN_AND_STORE, eslCallback, NULL, &err);
			}

			if (ret) {
				// Scan start(Save stored images)
				ret = eslib.eslExecute(instance, ET_SAVE_STORED_IMAGE, eslCallback, NULL, &err);
			}

			// Close scanner
			ret = eslib.eslCloseScanner(instance, &err);
		}

		// Free Library
		eslib.eslFree(instance, &err);
	}

	return err;
}

DWORD SCAN_OBR_OCR_GET_Default(void)
{
	DWORD instance = 0;
	DWORD err = 0;

	CEslStub eslib(NULL);

	//
	// Initialize Library
	//
	BOOL ret = eslib.eslInit(&instance, LICENSE_KEY, &err);
	if (ret) {

		//
		// Open Scanner
		//
		ConnectInfo scanner = {CT_DEFAULT, DEVICE_NAME}; // specify connection type and device name
		ret = eslib.eslOpenScanner(instance, &scanner, &err);
		if (ret) {

			if (ret) {
				// Scan start(Scan and Store)
				ret = eslib.eslExecute(instance, ET_SCAN_AND_STORE, eslCallback, NULL, &err);
			}

			if (ret) {
				// Scan start(Get OBR Data)
				ret = eslib.eslExecute(instance, ET_GET_OBR_DATA, eslCallback, NULL, &err);
			}

			if (ret) {
				// Scan start(Get OCR Data)
				ret = eslib.eslExecute(instance, ET_GET_OCR_DATA, eslCallback, NULL, &err);
			}

			if (ret) {
				// Scan start(Get Image Data)
				ret = eslib.eslExecute(instance, ET_GET_STORED_IMAGE, eslCallback, NULL, &err);
			}

			// Close scanner
			ret = eslib.eslCloseScanner(instance, &err);
		}

		// Free Library
		eslib.eslFree(instance, &err);
	}

	return err;
}

DWORD SCAN_SAVE_JPEG(void)
{
	DWORD instance = 0;
	DWORD err = 0;

	CEslStub eslib(NULL);

	//
	// Initialize Library
	//
	BOOL ret = eslib.eslInit(&instance, LICENSE_KEY, &err);
	if (ret) {

		//
		// Open Scanner
		//
		ConnectInfo scanner = {CT_DEFAULT, DEVICE_NAME}; // specify connection type and device name
		ret = eslib.eslOpenScanner(instance, &scanner, &err);
		if (ret) {
			//
			// Update Scan Basic parameters
			//
			EslParamScanBasic scan_params;
			scan_params.type = ESL_PARAM_SCAN_BASIC;
			scan_params.size = sizeof(scan_params);

			// Get default parameters
			ret = eslib.eslCtrlParameter(instance, POT_GET_DEFAULT, (pEslParamHdr)&scan_params, &err);

			// Change some parameters...
			if (ret) {
				scan_params.docSource = SRC_AUTO;
				scan_params.imgType = IT_COLOR;
				scan_params.resolution = 300;
				scan_params.docSize = DS_A4;
				//scan_params.sizeUnit = SU_INCH;
				//scan_params.sizeUser.left = 0;
				//scan_params.sizeUser.top = 0;
				//scan_params.sizeUser.right = 500;
				//scan_params.sizeUser.bottom = 500;
				scan_params.docRotate = DR_NONE;
				scan_params.brightness = 0;
				scan_params.contrast = 0;
				scan_params.threshold = 110;
				scan_params.colorFilter = CF_NONE;
				scan_params.numScan = 5;

				// Set parameters
				ret = eslib.eslCtrlParameter(instance, POT_SET, (pEslParamHdr)&scan_params, &err);
			}

			if (ret) {
				// Scan start(Scan and Store)
				ret = eslib.eslExecute(instance, ET_SCAN_AND_STORE, eslCallback, NULL, &err);
			}

			//
			// Update Save Basic parameters
			//
			EslParamSaveBasic save_params;
			save_params.type = ESL_PARAM_SAVE_BASIC;
			save_params.size = sizeof(save_params);

			// Get default parameters
			if (ret) {
				ret = eslib.eslCtrlParameter(instance, POT_GET_DEFAULT, (pEslParamHdr)&save_params, &err);
			}

			// Change some parameters...
			if (ret) {
				save_params.fileFormat = FF_JPEG;
				StringCbCopy(save_params.filePath, sizeof(save_params.filePath), TEXT("C:"));
				StringCbCopy(save_params.fileNamePrefix, sizeof(save_params.fileNamePrefix), TEXT("img"));
				save_params.fileTimeFormat = TF_YYYYMMDD_HHMMSS;
				save_params.fileNumCounterDigits = 4;
				save_params.fileNumStartCount = 1;

				// Set parameters
				ret = eslib.eslCtrlParameter(instance, POT_SET, (pEslParamHdr)&save_params, &err);
			}

			//
			// Update JPEG file parameters
			//
			EslParamSaveJPEG jpeg_params;
			jpeg_params.type = ESL_PARAM_SAVE_JPEG;
			jpeg_params.size = sizeof(jpeg_params);

			// Get default parameters
			if (ret) {
				ret = eslib.eslCtrlParameter(instance, POT_GET_DEFAULT, (pEslParamHdr)&jpeg_params, &err);
			}

			// Change some parameters...
			if (ret) {
				jpeg_params.imgQuality = 50;
				jpeg_params.encProgressive = FALSE;

				// Set parameters
				ret = eslib.eslCtrlParameter(instance, POT_SET, (pEslParamHdr)&jpeg_params, &err);
			}

			if (ret) {
				// Scan start(Save stored images)
				ret = eslib.eslExecute(instance, ET_SAVE_STORED_IMAGE, eslCallback, NULL, &err);
			}

			// Close scanner
			ret = eslib.eslCloseScanner(instance, &err);
		}

		// Free Library
		eslib.eslFree(instance, &err);
	}

	return err;
}

DWORD SCAN_SAVE_PDF(void)
{
	DWORD instance = 0;
	DWORD err = 0;

	CEslStub eslib(NULL);

	//
	// Initialize Library
	//
	BOOL ret = eslib.eslInit(&instance, LICENSE_KEY, &err);
	if (ret) {

		//
		// Open Scanner
		//
		ConnectInfo scanner = {CT_DEFAULT, DEVICE_NAME}; // specify connection type and device name
		ret = eslib.eslOpenScanner(instance, &scanner, &err);
		if (ret) {
			//
			// Update Scan Basic parameters
			//
			EslParamScanBasic scan_params;
			scan_params.type = ESL_PARAM_SCAN_BASIC;
			scan_params.size = sizeof(scan_params);

			// Get default parameters
			ret = eslib.eslCtrlParameter(instance, POT_GET_DEFAULT, (pEslParamHdr)&scan_params, &err);

			// Change some parameters...
			if (ret) {
				scan_params.docSource = SRC_AUTO;
				scan_params.imgType = IT_COLOR;
				scan_params.resolution = 300;
				scan_params.docSize = DS_A4;
				//scan_params.sizeUnit = SU_INCH;
				//scan_params.sizeUser.left = 0;
				//scan_params.sizeUser.top = 0;
				//scan_params.sizeUser.right = 500;
				//scan_params.sizeUser.bottom = 500;
				scan_params.docRotate = DR_AUTO;
				scan_params.brightness = 0;
				scan_params.contrast = 0;
				scan_params.threshold = 110;
				scan_params.colorFilter = CF_NONE;
				scan_params.numScan = 5;

				// Set parameters
				ret = eslib.eslCtrlParameter(instance, POT_SET, (pEslParamHdr)&scan_params, &err);
			}

			//
			// Update Save Basic parameters
			//
			EslParamSaveBasic save_params;
			save_params.type = ESL_PARAM_SAVE_BASIC;
			save_params.size = sizeof(save_params);

			// Get default parameters
			if (ret) {
				ret = eslib.eslCtrlParameter(instance, POT_GET_DEFAULT, (pEslParamHdr)&save_params, &err);
			}

			// Change some parameters...
			if (ret) {
				save_params.fileFormat = FF_PDF;
				StringCbCopy(save_params.filePath, sizeof(save_params.filePath), TEXT("C:"));
				StringCbCopy(save_params.fileNamePrefix, sizeof(save_params.fileNamePrefix), TEXT("img"));
				save_params.fileTimeFormat = TF_NONE;
				save_params.fileNumCounterDigits = 4;
				save_params.fileNumStartCount = 1;

				// Set parameters
				ret = eslib.eslCtrlParameter(instance, POT_SET, (pEslParamHdr)&save_params, &err);
			}

			//
			// Update PDF file parameters
			//
			EslParamSavePDF pdf_params;
			pdf_params.type = ESL_PARAM_SAVE_PDF;
			pdf_params.size = sizeof(pdf_params);

			// Get default parameters
			if (ret) {
				ret = eslib.eslCtrlParameter(instance, POT_GET_DEFAULT, (pEslParamHdr)&pdf_params, &err);
			}

			// Change some parameters...
			if (ret) {
				pdf_params.pdfType = PT_NORMAL;
				pdf_params.addOcrText = TRUE;
				pdf_params.ocrLanguage = OL_EN;

				// Set parameters
				ret = eslib.eslCtrlParameter(instance, POT_SET, (pEslParamHdr)&pdf_params, &err);
			}

			if (ret) {
				// Scan start(Scan and Save)
				ret = eslib.eslExecute(instance, ET_SCAN_AND_SAVE, eslCallback, NULL, &err);
			}

			// Close scanner
			ret = eslib.eslCloseScanner(instance, &err);
		}

		// Free Library
		eslib.eslFree(instance, &err);
	}

	return err;
}

DWORD SCAN_STORE_SAVE_PDF_SEPARATION(void)
{
	DWORD instance = 0;
	DWORD err = 0;

	CEslStub eslib(NULL);

	//
	// Initialize Library
	//
	BOOL ret = eslib.eslInit(&instance, LICENSE_KEY, &err);
	if (ret) {

		//
		// Open Scanner
		//
		ConnectInfo scanner = {CT_DEFAULT, DEVICE_NAME}; // specify connection type and device name
		ret = eslib.eslOpenScanner(instance, &scanner, &err);
		if (ret) {
			//
			// Update Scan Basic parameters
			//
			EslParamScanBasic scan_params;
			scan_params.type = ESL_PARAM_SCAN_BASIC;
			scan_params.size = sizeof(scan_params);

			// Get default parameters
			ret = eslib.eslCtrlParameter(instance, POT_GET_DEFAULT, (pEslParamHdr)&scan_params, &err);

			// Change some parameters...
			if (ret) {
				scan_params.docSource = SRC_AUTO;
				scan_params.imgType = IT_COLOR;
				scan_params.resolution = 200;
				scan_params.docSize = DS_A4;
				//scan_params.sizeUnit = SU_INCH;
				//scan_params.sizeUser.left = 0;
				//scan_params.sizeUser.top = 0;
				//scan_params.sizeUser.right = 500;
				//scan_params.sizeUser.bottom = 500;
				scan_params.docRotate = DR_NONE;
				scan_params.brightness = 0;
				scan_params.contrast = 0;
				scan_params.threshold = 110;
				scan_params.colorFilter = CF_NONE;
				scan_params.numScan = 6;

				// Set parameters
				ret = eslib.eslCtrlParameter(instance, POT_SET, (pEslParamHdr)&scan_params, &err);
			}

			//
			// Update Save Basic parameters
			//
			EslParamSaveExV1 save_params;
			save_params.type = ESL_PARAM_SAVE_EX_V1;
			save_params.size = sizeof(save_params);

			// Get default parameters
			if (ret) {
				ret = eslib.eslCtrlParameter(instance, POT_GET_DEFAULT, (pEslParamHdr)&save_params, &err);
			}

			// Change some parameters...
			if (ret) {
				save_params.fileFormat = FF_MULTITIFF;
				StringCbCopy(save_params.filePath, sizeof(save_params.filePath), TEXT("C:"));
				StringCbCopy(save_params.fileNamePrefix, sizeof(save_params.fileNamePrefix), TEXT("img"));
				save_params.fileTimeFormat = TF_YYYYMMDD_HHMMSS;
				save_params.fileNumCounterDigits = 4;
				save_params.fileNumStartCount = 1;
				save_params.assortType = AT_PAGECOUNT;
				save_params.assortDivPageCount = 3;

				// Set parameters
				ret = eslib.eslCtrlParameter(instance, POT_SET, (pEslParamHdr)&save_params, &err);
			}

			//
			// Update PDF file parameters
			//
			EslParamSavePDF pdf_params;
			pdf_params.type = ESL_PARAM_SAVE_PDF;
			pdf_params.size = sizeof(pdf_params);

			// Get default parameters
			if (ret) {
				ret = eslib.eslCtrlParameter(instance, POT_GET_DEFAULT, (pEslParamHdr)&pdf_params, &err);
			}

			// Change some parameters...
			if (ret) {
				pdf_params.pdfType = PT_NORMAL;
				pdf_params.addOcrText = FALSE;
				pdf_params.ocrLanguage = OL_EN;

				// Set parameters
				ret = eslib.eslCtrlParameter(instance, POT_SET, (pEslParamHdr)&pdf_params, &err);
			}

			if (ret) {
				// Scan start(Scan and Store)
				ret = eslib.eslExecute(instance, ET_SCAN_AND_STORE, eslCallback, NULL, &err);
			}

			if (ret) {
				// Scan start(Save stored images)
				ret = eslib.eslExecute(instance, ET_SAVE_STORED_IMAGE, eslCallback, NULL, &err);
			}

			// Close scanner
			ret = eslib.eslCloseScanner(instance, &err);
		}

		// Free Library
		eslib.eslFree(instance, &err);
	}

	return err;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	SCAN_SAVE_Default();
	SCAN_SAVE_with_OBRFileName();
	SCAN_SAVE_with_OBRFolderName();

//	SCAN_STORE_SAVE_Default();
//	SCAN_OBR_OCR_GET_Default();
//	SCAN_SAVE_JPEG();
//	SCAN_SAVE_PDF();
//	SCAN_STORE_SAVE_PDF_SEPARATION();

	return 0;
}

void debugSave(HANDLE hDIB, CString FileName)
{
	ES_TRACE_FUNC(TEXT("CDIBImage::debugSave"));

	if (hDIB == NULL) {
		return;
	}

	CAtlFile bmp;
	HRESULT hr = bmp.Create(FileName, GENERIC_WRITE, 0, CREATE_ALWAYS);
	if (SUCCEEDED(hr)) {
		DWORD hdr_size = 0;
		DWORD img_size = 0;

		PBYTE pDIB = (PBYTE)::GlobalLock(hDIB);
		if (pDIB) {
			BITMAPINFOHEADER* pbmih = (BITMAPINFOHEADER*)pDIB;
			hdr_size = sizeof(BITMAPINFOHEADER) + (pbmih->biClrUsed * sizeof(RGBQUAD));
			img_size = pbmih->biSizeImage;

			BITMAPFILEHEADER bmfh = {0};
			// Initialize the remaining BITMAPINFOHEADER fields
			bmfh.bfType	 = ((WORD) ('M' << 8) | 'B');
			bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + hdr_size;
			bmfh.bfSize	 = bmfh.bfOffBits + img_size;

			bmp.Write(&bmfh, sizeof(BITMAPFILEHEADER));
			bmp.Write(pDIB, hdr_size + img_size);

			::GlobalUnlock(hDIB);
		}
	}
}
