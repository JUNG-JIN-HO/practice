
// FileSeparationDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "FileSeparation.h"
#include "FileSeparationDlg.h"
#include "afxdialogex.h"
#include <filesystem>
#include <string>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace fs = std::filesystem;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileSeparationDlg 대화 상자



CFileSeparationDlg::CFileSeparationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILESEPARATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileSeparationDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PROGRESS1, m_progressBar);
}

BEGIN_MESSAGE_MAP(CFileSeparationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CFileSeparationDlg::OnBnClickedMfcbutton1)
    ON_COMMAND(ID_OPEN_FOLDER, &CFileSeparationDlg::OnOpenFolder)
END_MESSAGE_MAP()


// CFileSeparationDlg 메시지 처리기

BOOL CFileSeparationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

    m_progressBar.SetRange(0, 100);
    m_progressBar.SetPos(0);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CFileSeparationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CFileSeparationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CFileSeparationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFileSeparationDlg::UpdateProgress(int value)
{
    m_progressBar.SetPos(value);
}

void CFileSeparationDlg::OnBnClickedMfcbutton1()
{
    CFolderPickerDialog folderDlg(nullptr, OFN_FILEMUSTEXIST, this);
    if (folderDlg.DoModal() == IDOK)
    {
        CString srcPath = folderDlg.GetPathName();
        CString retPath = srcPath + "_Precision";

        if (!fs::exists((LPCWSTR)retPath))
        {
            fs::create_directory((LPCWSTR)retPath);
        }

        // 총 파일 수 계산
        int total_files = 0;
        for (const auto& entry : fs::directory_iterator((LPCWSTR)srcPath))
        {
            for (const auto& file : fs::directory_iterator(entry.path()))
            {
                if (file.path().extension() == ".bmp")
                {
                    total_files++;
                }
            }
        }

        if (total_files == 0)
        {
            AfxMessageBox(L"처리할 파일이 없습니다.");
            return;
        }

        int processed_files = 0;
        int index = 0;
        for (const auto& entry : fs::directory_iterator((LPCWSTR)srcPath))
        {
            if (entry.path().extension() == ".csv")
                continue;

            for (const auto& file : fs::directory_iterator(entry.path()))
            {
                if (file.path().extension() == ".bmp")
                {
                    std::wstring filename = file.path().filename().wstring();
                    std::vector<std::wstring> split;
                    size_t pos = 0, found;
                    while ((found = filename.find_first_of(L'_', pos)) != std::wstring::npos)
                    {
                        split.push_back(filename.substr(pos, found - pos));
                        pos = found + 1;
                    }
                    split.push_back(filename.substr(pos));

                    if (std::find(split.begin(), split.end(), L"Target") != split.end())
                    {
                        size_t targetIndex = std::distance(split.begin(), std::find(split.begin(), split.end(), L"Target"));
                        std::vector<std::wstring> remaining_elements(split.begin() + targetIndex + 1, split.end());

                        std::wstring degree, fieldx, fieldy, site_x, site_y, site_num, str_name;
                        CString degree_folder_CString;
                        if (remaining_elements.size() == 5)
                        {
                            degree = remaining_elements[0];
                            fieldx = remaining_elements[1];
                            fieldy = remaining_elements[2];
                            site_x = remaining_elements[3];
                            site_y = remaining_elements[4];
                        }
                        else if (remaining_elements.size() == 6)
                        {
                            degree = remaining_elements[0];
                            fieldx = remaining_elements[1];
                            fieldy = remaining_elements[2];
                            site_num = remaining_elements[3];
                            site_x = remaining_elements[4];
                            site_y = remaining_elements[5];
                        }
                        else
                        {
                            CString message;
                            message.Format(L"Unexpected format in file: %s", filename.c_str());
                            AfxMessageBox(message);
                            continue;
                        }

                        site_y = site_y.substr(0, site_y.find_last_of(L"."));
                        str_name = L"Target_" + fieldx + L"_" + fieldy + L"_" + site_x + L"_" + site_y;

                        // CString을 사용하여 경로 생성
                        degree_folder_CString.Format(L"%s\\%s\\%s", (LPCWSTR)retPath, str_name.c_str(), degree.c_str());

                        std::wstring degree_folder = degree_folder_CString.GetString();

                        if (!fs::exists(degree_folder))
                        {
                            fs::create_directories(degree_folder);
                        }

                        fs::copy(file.path(), degree_folder, fs::copy_options::overwrite_existing);

                        std::wstring name = degree_folder + L"\\" + filename;
                        std::wstring rename = degree_folder + L"\\" + std::to_wstring(index) + L"_" + str_name + L".bmp";
                        if (fs::exists(rename))
                        {
                            fs::remove(rename);
                        }

                        fs::rename(name, rename);

                        
                    }
                }

                // 진행 상태 업데이트
                processed_files++;
                int progress = static_cast<int>((processed_files * 100) / total_files);
                UpdateProgress(progress);
            }
            index++;
        }

        AfxMessageBox(L"작업이 완료되었습니다.");
    }
}

void CFileSeparationDlg::OnOpenFolder()
{
    CFolderPickerDialog folderDlg(nullptr, OFN_FILEMUSTEXIST, this);
    if (folderDlg.DoModal() == IDOK)
    {
        CString srcPath = folderDlg.GetPathName();
        CString retPath = srcPath + "_Precision";

        if (!fs::exists((LPCWSTR)retPath))
        {
            fs::create_directory((LPCWSTR)retPath);
        }

        int index = 0;

        for (const auto& entry : fs::directory_iterator((LPCWSTR)srcPath))
        {
            if (entry.path().extension() == ".csv")
                continue;

            for (const auto& file : fs::directory_iterator(entry.path()))
            {
                if (file.path().extension() == ".bmp")
                {
                    std::wstring filename = file.path().filename().wstring();
                    std::vector<std::wstring> split;
                    size_t pos = 0, found;
                    while ((found = filename.find_first_of(L'_', pos)) != std::wstring::npos)
                    {
                        split.push_back(filename.substr(pos, found - pos));
                        pos = found + 1;
                    }
                    split.push_back(filename.substr(pos));

                    if (std::find(split.begin(), split.end(), L"Target") != split.end())
                    {
                        size_t targetIndex = std::distance(split.begin(), std::find(split.begin(), split.end(), L"Target"));
                        std::vector<std::wstring> remaining_elements(split.begin() + targetIndex + 1, split.end());

                        std::wstring degree, fieldx, fieldy, site_x, site_y, site_num, str_name;
                        CString degree_folder_CString;
                        if (remaining_elements.size() == 5)
                        {
                            degree = remaining_elements[0];
                            fieldx = remaining_elements[1];
                            fieldy = remaining_elements[2];
                            site_x = remaining_elements[3];
                            site_y = remaining_elements[4];
                        }
                        else if (remaining_elements.size() == 6)
                        {
                            degree = remaining_elements[0];
                            fieldx = remaining_elements[1];
                            fieldy = remaining_elements[2];
                            site_num = remaining_elements[3];
                            site_x = remaining_elements[4];
                            site_y = remaining_elements[5];
                        }
                        else
                        {
                            CString message;
                            message.Format(L"Unexpected format in file: %s", filename.c_str());
                            AfxMessageBox(message);
                            continue;
                        }

                        site_y = site_y.substr(0, site_y.find_last_of(L"."));
                        str_name = L"Target_" + fieldx + L"_" + fieldy + L"_" + site_x + L"_" + site_y;

                        // CString을 사용하여 경로 생성
                        degree_folder_CString.Format(L"%s\\%s\\%s", (LPCWSTR)retPath, str_name.c_str(), degree.c_str());

                        std::wstring degree_folder = degree_folder_CString.GetString();

                        if (!fs::exists(degree_folder))
                        {
                            fs::create_directories(degree_folder);
                        }

                        fs::copy(file.path(), degree_folder, fs::copy_options::overwrite_existing);

                        std::wstring name = degree_folder + L"\\" + filename;
                        std::wstring rename = degree_folder + L"\\" + std::to_wstring(index) + L"_" + str_name + L".bmp";
                        if (fs::exists(rename))
                        {
                            fs::remove(rename);
                        }

                        fs::rename(name, rename);
                    }
                }
            }
            index++;
        }

        AfxMessageBox(L"작업이 완료되었습니다.");
    }
}
