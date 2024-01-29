# 安全中心|deepin-defender|

## 概述

安全中心是一款提供系統安全檢測、病毒查殺、防火牆、垃圾清理以及安全工具等功能的應用工具。



## 使用入門

您可以透過以下方式執行或關閉安全中心，或者建立捷徑。

### 執行安全中心

1. 單擊任務欄上的啟動器 ![deepin-launcher](../common/deepin_launcher.svg)，進入啟動器介面。
2. 上下滾動滑鼠滾輪瀏覽或透過搜尋，找到「安全中心」按鈕 ![deepin_defender](../common/deepin_defender.svg)，單擊開啟。
3. 右鍵單擊 ![deepin_defender](../common/deepin_defender.svg)，您可以：
   - 單擊 **建立桌面捷徑**，在桌面建立捷徑。
   - 單擊 **釘選到Dock**，將應用程式固定到Dock。
   - 單擊 **開機啟動**，將應用程式添加到開機啟動項，在電腦開機時自動執行該應用程式。

### 關閉安全中心

- 右鍵單擊工具列上的 ![deepin_defender](../common/deepin_defender.svg) 圖示，選擇 **退出** 來關閉安全中心。
- 在安全中心介面單擊 ![icon_menu](../common/icon_menu.svg)，選擇 **退出** 來關閉安全中心。

## 操作介紹

### 系統體檢

1. 打開安全中心，選擇左側導航欄的 **系統體檢**，在首頁可以單擊 **立即體檢**，進行系統體檢。

2. 體檢完後，若有異常則可以對每個問題項單獨操作，還可以單擊 **一鍵修復** 快速修復多項問題。

![0|result](fig/result.png)

3. 處理完成後，單擊 **完成** 返回到系統體檢首頁。

### 病毒查殺

#### 病毒掃描

安全中心支援三種病毒掃描方式，分別為全盤掃描、快速掃描和自訂掃描。

1. 打開安全中心，選擇左側導航欄的 **病毒查殺**。在病毒查殺介面，選擇病毒掃描方式，掃描完成後會顯示掃描結果。

   ![0|virus-scan](fig/virus_scan.png)

2. 根據掃描結果，可以對每個風險項單獨操作，也可以選擇批量操作。有以下幾個操作項：

   **單項操作**：
   
   - 修復：單擊修復按鈕 ![repair](../common/repair.svg) 後，風險項被修復，下次掃描不會被判定為異常。

   - 隔離：單擊隔離按鈕 ![isolation](../common/isolation.svg) 後，風險項被隔離，隔離後不會影響其他文件，下次掃描不會被判定為異常，可在隔離區查看。

   - 信任：單擊信任按鈕 ![trust](../common/trust.svg) 後，風險項被信任，下次掃描不會被判定為異常，可在信任區查看。

   **批次操作**：
   
   勾選需要處理的風險項後，在頁面底部的下拉框中選擇 **立即修復**、**立即隔離** 或 **添加信任** 選項後，單擊 **執行**，則可以批次處理風險項。

   ![0|virus-result](fig/virusresult.png)



#### 定時掃描

針對快速掃描和全盤掃描，還可以設定定時掃描。定時掃描功能預設關閉，開啟後，可以根據設置的內容執行病毒掃描任務。

- 掃描類型：支援選擇快速掃描或全盤掃描。
- 掃描時間：選擇或自訂掃描時間。
- 發現病毒處理方式：
   - 詢問：發現病毒後彈出處理提示，可以選擇 **忽略**，不進行處理，還可以選擇 **立即處理**。若不做任何處理，則預設不處理。
   - 自動處理：掃描完成後自動處理異常病毒。

![0|timed_scan](fig/timed_scan.png)

#### USB掃描

對於USB儲存裝置，可以設定是否自動掃描及發現病毒處理方式。

- 詢問：發現病毒後，可以選擇 **立即處理** 將病毒隔離或選擇 **取消** 退出掃描。
- 自動處理：掃描完成後自動處理異常病毒。



#### 隔離區

在病毒查殺界面，單擊 **隔離區**，可查看隔離文件列表，還可以選擇將文件移出隔離。

![0|quarantine-area](fig/quarantinearea.png)

#### 信任區

在病毒查殺界面，單擊 **信任區**，可查看信任文件列表，還可以選擇將文件移出信任。

![0|trust-area](fig/trustarea.png)



#### 白名單

在病毒查殺介面，單擊 **白名單**，將文件加入白名單後，病毒掃描時則會被跳過。若文件發生變化，將導致信任失效，並自動從白名單目錄中移除。

#### 查殺日誌

在病毒查殺界面，單擊 **查殺日誌**，可查看每次病毒掃描的時間、掃描類型及結果。單擊詳情按鈕 ![icon-details](../common/icon_details.svg)，可查看日誌詳情，還可以選擇刪除日誌。

![0|log](fig/log.png)

#### 病毒庫更新

每天都有新病毒產生，所以要定期更新病毒庫，以滿足電腦的安全需求，目前支援病毒庫線上更新和離線更新。

**線上更新**

當檢測到新的病毒庫時，在病毒查殺介面，單擊 **更新** 進行病毒庫更新。如果沒有檢測到新的病毒庫，可以單擊 **檢查更新** 進行檢查。

**離線更新**

如果因為斷網等情況無法線上更新病毒庫，則可以選擇離線更新。

1. 在安全中心介面，單擊 ![icon_menu](../common/icon_menu.svg)。
2. 選擇 **病毒庫離線更新**，彈出檔案管理器，匯入對應的.zip格式病毒庫文件。如果目前使用的殺毒引擎是瑞星網安，則必須匯入瑞星網安病毒庫文件。
3. 匯入成功後，病毒查殺介面右下角的病毒庫版本更新為匯入後的版本號。

### 防火牆

打開安全中心，選擇左側導航欄的 **防火牆**。在防火牆介面，開啟或關閉防火牆需要輸入系統登入密碼進行認證。

防火牆開關預設關閉，網路防護策略無法生效。開啟後預設選中專用網路策略，也可以切換為公共網路策略或自訂網路防護規則。

>  ![notes](../common/notes.svg) 說明：當系統安全等級切換為高或中時，防火牆開關則會開啟。

- 公共網路防護：主要為車站、商場、咖啡廳等公共網路訊號，執行公網場景下的預設策略。

- 專用網路防護：主要為辦公、家庭網路場景，執行專網場景下的預設策略。

- 自訂網路防護規則：選擇此項後，您可以自行配置進階防火牆規則。

![0|firewall](fig/firewall.png)

#### 自訂規則

1. 在防火牆介面，勾選 **自訂**，單擊更多圖示 >，進入配置介面。

2. 單擊 **添加** 按鈕，填寫或勾選防火牆規則訊息，包括規則名稱、程式服務、策略、方向。協議等。

   - 規則名稱：規則名稱不可重複，可以使用預設名稱或自訂名稱。

   - 程式服務：預設選擇全部服務，也可以在下拉框中勾選或在輸入框模糊搜尋對應的服務。

   - 策略：預設選擇允許，表示允許封包傳輸。選擇拒絕，表示不允許封包傳輸。

   - 方向：預設選擇出入站，出站表示本機主機訪問外部資料，入站表示外部資料進入本機主機。

   - 協議：系統支援的通信協議，預設選擇TCP，也可以在下拉框中勾選或在輸入框模糊搜尋對應的協議。

   - 本機埠/IP：非必填項，表示本機主機的埠號和IP，填寫規則可以參考範例。

   - 遠端埠/IP：非必填項，表示遠端主機的埠號和IP，填寫規則可以參考範例。

   - 狀態：預設不勾選，表示該規則已停用。勾選後，啟用該規則。

   ![0|rule1](fig/rule1.png)

3. 完成所有的訊息後，單擊 **保存** 按鈕。添加成功後，在配置頁面查看規則列表。

   ![0|rule2](fig/rule2.png)

4. 如果添加了多個規則，預設已啟用的規則優先顯示在前面並按照建立時間降序排列，已停用的規則在後面顯示且按照建立規則時間的降序排列。當先後建立的規則有衝突時，以最後建立的啟用規則為準。

#### 右鍵選單

對於已添加的規則，根據啟用狀態可以進行相關操作。

單項操作：

- 已啟用的規則：在右鍵選單中，只能執行停用操作。
- 已停用的規則：在右鍵選單中，可以執行修改、啟用或刪除操作。

多項操作：

- 全為已啟用的規則：在右鍵選單中，只能執行停用操作。
- 全為已停用的規則：在右鍵選單中，可以執行啟用或刪除操作。
- 包含已啟用和已停用的規則：在右鍵選單中，可以執行啟用、停用或刪除操作。

### 垃圾清理

作業系統在日常執行中會產生各種垃圾，當垃圾越來越多，會影響系統的執行效率，浪費磁碟資源。建議定期清理垃圾，保障系統執行流暢，提升資源利用率。

1. 打開安全中心，選擇左側導航欄的 **垃圾清理**，進入垃圾清理介面。
2. 勾選對應的垃圾類型，目前可以選擇清理系統垃圾、應用垃圾、痕跡訊息和Cookies。

![0|garbage_cleaning](fig/cleanup.png)

3. 單擊 **立即掃描**，進入掃描介面。
4. 掃描完成後，可查看掃描發現的垃圾，並進行相應的處理。
   
   - 立即清理：清理所有選中的垃圾，您還可以自行取消勾選或者增加勾選。
   - 返回：不做任何處理，返回到垃圾清理介面。

5. 清理完成後單擊 **完成** 即可，清理過程中還可以單擊 **取消清理**。


### 安全工具

打開安全中心，選擇左側導航欄的 **安全工具**。安全工具包括系統工具和網路工具，單擊對應圖示則可以使用小工具。

![0|security_tool](fig/security_tool.png)

#### 系統工具

**USB安全**

在USB安全介面，可以查看帶儲存功能的 USB 裝置連接電腦的紀錄。

![0|usb_conneting](fig/usbconnection.png)

還可以將常用的USB裝置加入白名單，設定只允許白名單裝置連接電腦，防止電腦資料洩露。



**登入安全**

在登入安全介面，可以設定密碼安全等級，還可以設定密碼到期提醒時間，定期更換密碼。

- 安全等級為 **高** 或 **中** ：在控制中心修改密碼，或建立新使用者設定密碼時，若設定的密碼不符合級別要求，會儲存失敗，您可以前往安全中心修改等級或重新設定密碼。
- 安全等級為 **低** ：在控制中心修改密碼，或建立新使用者設定密碼的時候，直接儲存新設定的密碼。

![0|login-security](fig/login_safety.png)

**自啟動應用**

開機自啟動管理介面僅顯示啟動器裡的應用，包括應用名稱、自啟動狀態和操作按鈕。每個應用可選擇允許或禁止開機自啟動。

![0|self_launch_management](fig/startup.png)

**系統安全等級**

系統安全等級配置功能入口預設隱藏，啟用後會加強對系統權限的控制。如果需要使用，可以聯系統信技術支援獲取開啟方式。

根據使用場景，選擇適合系統安全等級配置，預設系統安全等級為低。

- **系統安全等級-高**：開啟等保三級的安全防護，開啟身份鑑別、自主訪問控制、標記和強制訪問控制、安全審計、資料完整性校驗、資料保密性、網路安全、執行安全、資源監控等安全保護。同時開啟三權分立，配置系統管理員、系統審計管理員、系統安全管理員帳號密碼。
- **系統安全等級-中**：開啟系統基礎安全保護，同時開啟防火牆網路安全保護、密碼安全等級調整為中，保護系統安全。
- **系統安全等級-低**：適用於可信任的使用環境，提供基礎安全防護。

（1）系統安全等級由低或中切換為高

1. 在系統安全等級介面，勾選 **高**。彈出授權框，輸入系統登入密碼進行認證。

   ![0|certification](fig/certification.png)

2. 彈出二次確認視窗，單擊 **確定**。配置系統管理員、審計管理員、安全管理員帳號密碼，每個管理員分配不同的權限。

   - 系統管理員（sysadm）：建立、刪除使用者，安裝軟體，系統相關設定；

   - 審計管理員（audadm）：添加審計規則，查看審計日誌；

   - 安全管理員（secadm）：管理使用者安全上下文，查詢、設定安全配置；

   ![0|adm](fig/adm.png)

4. 配置成功後，重啟系統生效，普通使用者部分權限會受限。在系統登入介面會顯示三個管理員帳戶，可以進行切換。

   ![0|login](fig/login.png)

（2）系統安全等級由高切換為中或低

只有在安全管理員帳戶下，系統安全等級可以從高切換為中或低。系統管理員和審計管理員帳戶都無法切換系統安全等級。

1. 如果目前不是安全管理員帳戶，在系統登入介面單擊切換使用者圖示![userswitch](../common/userswitch.svg)，選擇secadm使用者並登入。

2. 進入系統安全等級介面，勾選 **中** 或 **低**。彈出授權框，輸入系統登入密碼進行認證。

3. 二次確認，配置成功後，刪除三權帳戶、復原普通使用者權限，等級切換立即生效。系統重啟後復原系統安全相關配置。

（3）系統安全等級由低切換為中或由中切換為低

1. 在系統安全等級介面，由低切換為中或由中切換為低時，需要系統登入密碼進行認證。
2. 二次確認，配置成功後，等級切換立即生效。

**可信保護**

主要對系統關鍵元件進行完整性校驗，可信保護功能預設關閉。

當系統安全等級切換為高時，同步開啟完整性檢查開關；當系統安全等級切換為中或低時，同步關閉完整性檢查開關，可以自行關閉或開啟。

1. 在可信保護介面，預設提供部分受保護的系統檔案，不可修改，不可移除，保護策略預設禁止。對於新增的元件文件，則可以進行修改、移除保護，設定保護策略等操作。

2. 單擊 **添加** 按鈕，彈出檔案管理器視窗，可以手動添加需保護的元件文件。

   ![0|trusted_protection](fig/trusted_protection.png)

3. 新增的元件文件保護策略預設為「警告」，也可以設定為「禁止」。

   - 警告：當完整性被破壞且被檢測到執行時，發出系統通知「xxx元件完整性被破壞，是否允許執行」，您可以選擇 **忽略** 或 **禁止**。
   - 禁止：當完整性被破壞且被檢測到執行時，發出系統通知「xxx元件完整性被破壞，禁止執行」。

4. 如果元件文件被修改了，狀態欄從「正常」變為「已篡改」。

5. 勾選對應的元件文件，單擊 **移除保護**，移除後則不再受保護。 



#### 網路工具

**聯網控制**

在聯網控制介面，聯網控制功能預設關閉。開啟後，可以設定列表中單個應用聯網的狀態，每個應用或服務下拉框有三種選項。

- 詢問：應用聯網時給出提示「xxx需要連接網路」，您可以選擇 **僅允許本次**、**始終允許** 或 **禁止**。 

- 允許：應用聯網時預設允許，不會有提示訊息。

- 禁止：應用聯網時給出提示「防火牆已禁止xxx連接網路」，您可以選擇 **前往設定** 或 **確定**。

> ![notes](../common/notes.svg) 說明：如果不發起網路連接則不會有提示訊息，比如本身就不會聯網的應用，即使選擇 **詢問** 或 **禁止** 也不會在啟動時彈出提示。

  ![0|network_control](fig/internetcontrol.png)

**流量監控**

在流量監控介面，流量監控功能預設關閉。開啟後，可以對系統裡應用上網所使用的流量情況進行監控。

- 目前聯網：查看列表中應用目前的下行網速、上行網速。單擊管控下拉框，可以選擇 **詢問**、**允許** 或 **禁止** 選項，控制應用是否能聯網，詳細操作請參考安全工具中的 [聯網控制](#網路工具)。

  ![0|current_network](fig/datausage.png)

- 流量排名：查看啟動器裡所有應用的流量排名，包括下行流量、上行流量、總流量、流量時間分布詳情。列表右上角支援切換時間跨度：當天、昨天、當月、上月。

  ![rankings](fig/rankings.png)


## 主選單

在主選單中，您可以進行相關參數設定、問題回饋、切換視窗主題，查看說明手冊等操作。

### 設定

在安全中心介面，單擊 ![icon_menu](../common/icon_menu.svg)，選擇 **設定** ，您可以進行基礎設定及安全設定。

#### 基礎設定

**病毒查殺**

- 加入「病毒查殺」到文件右鍵選單：勾選後，單擊文件（夾）右鍵可直接選擇 **病毒查殺**。
- 不掃描超過此大小的壓縮包：勾選後，若壓縮包超過設定的大小，則不會進行掃描。
- 病毒庫升級地址：勾選後，可設定病毒庫升級地址。

  

**殺毒引擎**

可以選擇瑞星網安或安恆訊息殺毒引擎。

**關閉主視窗**

- 選擇 **最小化至系統列**，關閉主視窗時，應用將隱藏到系統工具列。
- 選擇 **退出**，關閉主視窗時直接退出應用。
- 選擇**每次詢問**，每次關閉主視窗時會彈出確認彈框。

#### 還原預設值

單擊 **還原預設值** ，可復原目前頁面為預設設定值。

### 使用者回饋

1. 在安全中心介面，單擊 ![icon_menu](../common/icon_menu.svg)。
2. 選擇 **使用者回饋**，跳轉到「服務與支持」應用的「留言諮詢」介面，您可以回饋相關問題。

### 安全日誌

1. 在安全中心介面，單擊 ![icon_menu](../common/icon_menu.svg)。

2. 選擇 **安全日誌**，可以根據日誌類型和日期查看安全中心的操作日誌，還可以選擇清除日誌。

   ![0|security_log](fig/security_logs.png)

### 主題

視窗主題包含亮色主題、暗色主題和系統主題。

1. 在安全中心介面，單擊 ![icon_menu](../common/icon_menu.svg)。
2. 選擇 **主題**，選擇一個主題顏色。

### 說明

查看說明手冊，透過幫助進一步了解和使用安全中心。

1. 在安全中心介面，單擊 ![icon_menu](../common/icon_menu.svg)。
2. 選擇 **說明**，查看安全中心的說明手冊，進一步了解和使用安全中心。


### 關於

1. 在安全中心介面，單擊 ![icon_menu](../common/icon_menu.svg)。
2. 選擇 **關於**，查看安全中心的版本和介紹。

### 退出

1. 在安全中心介面，單擊 ![icon_menu](../common/icon_menu.svg)。
2. 選擇 **退出**。