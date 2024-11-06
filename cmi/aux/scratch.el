(setq aes-round-format "

# Round %d
vextracti128 $0x1, %%ymm%d, %%xmm%d
vaesimc %%xmm0, %%xmm0
aesdec %%xmm0, %%xmm14")

(let ((output ""))
  (dolist (n (number-sequence 0 7))
	(setq output
		  (concat
		   output
		   (format aes-round-format n n (- 15 n)))))
  (kill-new output))


(let ((output "")
	  (format ""))
  (dolist (n (number-sequence 16 31))
	(setq output
		  (concat
		   output
		   (format aes-round-format n n (- 15 n)))))
  (kill-new output)
  )



