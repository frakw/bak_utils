:: %1為映像檔名 %2為image ID 此2參數由exe自動載入
::--bwlimit 4M 4M為上傳頻寬 4M(Byte)
rclone copy %1 macrium_reflect:\C_bak\%2 -P --bwlimit 4M