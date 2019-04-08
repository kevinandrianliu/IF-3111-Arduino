# arduino

Tugas Besar IF3111 Platform Based Development - Arduino

# Deskripsi Umum

Sistem Arduino yang memanfaatkan ultrasonic sensor dan sound sensor untuk mendeteksi adanya seseorang dalam jarak tertentu, dan membunyikan alarm ketika terdeteksi. Selain itu, terdapat fitur tambahan seperti mengatur state dari sistem menggunakan keypad(state sistem ditampilkan dalam layar LCD dan 7-segment LCD), serta upload data terhadap server.

# Instalasi Aplikasi

- Download dari gitlab
- Buka file .ino menggunakan Arduino IDE
- Sambungkan rangkaian Arduino menggunakan USB
- 3 cara untuk menyambungkan kode dengan aplikasi: pilih tombol Upload pada tampilan(tanda panah); pilih Sketch -> Upload pada menu bar; atau tekan Ctrl+U dari keyboard
 

# Penggunaan Aplikasi

State aplikasi dapat diatur dengan keypad. Ada 4 jenis mode: Reset(status kalibrasi ulang), Standby(sensor tidak menyala), Armed(sensor menyala), dan Trigger(menyalakan sensor suara). Bila aplikasi berada dalam mode Armed dan terdapat suara dan benda/seseorang pada jarak tertentu, buzzer akan berbunyi.

# Deliverables

.fzz : board diagram Fritzing
.ino : kode program