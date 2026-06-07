
a.out:     file format elf64-x86-64


Disassembly of section .init:

00000000004007d8 <_init>:
  4007d8:	48 83 ec 08          	sub    $0x8,%rsp
  4007dc:	48 8b 05 15 18 20 00 	mov    0x201815(%rip),%rax        # 601ff8 <__gmon_start__>
  4007e3:	48 85 c0             	test   %rax,%rax
  4007e6:	74 05                	je     4007ed <_init+0x15>
  4007e8:	e8 73 00 00 00       	callq  400860 <__gmon_start__@plt>
  4007ed:	48 83 c4 08          	add    $0x8,%rsp
  4007f1:	c3                   	retq   

Disassembly of section .plt:

0000000000400800 <.plt>:
  400800:	ff 35 02 18 20 00    	pushq  0x201802(%rip)        # 602008 <_GLOBAL_OFFSET_TABLE_+0x8>
  400806:	ff 25 04 18 20 00    	jmpq   *0x201804(%rip)        # 602010 <_GLOBAL_OFFSET_TABLE_+0x10>
  40080c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400810 <waddch@plt>:
  400810:	ff 25 02 18 20 00    	jmpq   *0x201802(%rip)        # 602018 <waddch>
  400816:	68 00 00 00 00       	pushq  $0x0
  40081b:	e9 e0 ff ff ff       	jmpq   400800 <.plt>

0000000000400820 <wrefresh@plt>:
  400820:	ff 25 fa 17 20 00    	jmpq   *0x2017fa(%rip)        # 602020 <wrefresh>
  400826:	68 01 00 00 00       	pushq  $0x1
  40082b:	e9 d0 ff ff ff       	jmpq   400800 <.plt>

0000000000400830 <initscr@plt>:
  400830:	ff 25 f2 17 20 00    	jmpq   *0x2017f2(%rip)        # 602028 <initscr>
  400836:	68 02 00 00 00       	pushq  $0x2
  40083b:	e9 c0 ff ff ff       	jmpq   400800 <.plt>

0000000000400840 <cos@plt>:
  400840:	ff 25 ea 17 20 00    	jmpq   *0x2017ea(%rip)        # 602030 <cos@GLIBC_2.2.5>
  400846:	68 03 00 00 00       	pushq  $0x3
  40084b:	e9 b0 ff ff ff       	jmpq   400800 <.plt>

0000000000400850 <__libc_start_main@plt>:
  400850:	ff 25 e2 17 20 00    	jmpq   *0x2017e2(%rip)        # 602038 <__libc_start_main@GLIBC_2.2.5>
  400856:	68 04 00 00 00       	pushq  $0x4
  40085b:	e9 a0 ff ff ff       	jmpq   400800 <.plt>

0000000000400860 <__gmon_start__@plt>:
  400860:	ff 25 da 17 20 00    	jmpq   *0x2017da(%rip)        # 602040 <__gmon_start__>
  400866:	68 05 00 00 00       	pushq  $0x5
  40086b:	e9 90 ff ff ff       	jmpq   400800 <.plt>

0000000000400870 <endwin@plt>:
  400870:	ff 25 d2 17 20 00    	jmpq   *0x2017d2(%rip)        # 602048 <endwin>
  400876:	68 06 00 00 00       	pushq  $0x6
  40087b:	e9 80 ff ff ff       	jmpq   400800 <.plt>

0000000000400880 <wgetch@plt>:
  400880:	ff 25 ca 17 20 00    	jmpq   *0x2017ca(%rip)        # 602050 <wgetch>
  400886:	68 07 00 00 00       	pushq  $0x7
  40088b:	e9 70 ff ff ff       	jmpq   400800 <.plt>

0000000000400890 <wclear@plt>:
  400890:	ff 25 c2 17 20 00    	jmpq   *0x2017c2(%rip)        # 602058 <wclear>
  400896:	68 08 00 00 00       	pushq  $0x8
  40089b:	e9 60 ff ff ff       	jmpq   400800 <.plt>

00000000004008a0 <noecho@plt>:
  4008a0:	ff 25 ba 17 20 00    	jmpq   *0x2017ba(%rip)        # 602060 <noecho>
  4008a6:	68 09 00 00 00       	pushq  $0x9
  4008ab:	e9 50 ff ff ff       	jmpq   400800 <.plt>

00000000004008b0 <sin@plt>:
  4008b0:	ff 25 b2 17 20 00    	jmpq   *0x2017b2(%rip)        # 602068 <sin@GLIBC_2.2.5>
  4008b6:	68 0a 00 00 00       	pushq  $0xa
  4008bb:	e9 40 ff ff ff       	jmpq   400800 <.plt>

00000000004008c0 <wmove@plt>:
  4008c0:	ff 25 aa 17 20 00    	jmpq   *0x2017aa(%rip)        # 602070 <wmove>
  4008c6:	68 0b 00 00 00       	pushq  $0xb
  4008cb:	e9 30 ff ff ff       	jmpq   400800 <.plt>

00000000004008d0 <curs_set@plt>:
  4008d0:	ff 25 a2 17 20 00    	jmpq   *0x2017a2(%rip)        # 602078 <curs_set>
  4008d6:	68 0c 00 00 00       	pushq  $0xc
  4008db:	e9 20 ff ff ff       	jmpq   400800 <.plt>

Disassembly of section .text:

00000000004008e0 <_start>:
  4008e0:	31 ed                	xor    %ebp,%ebp
  4008e2:	49 89 d1             	mov    %rdx,%r9
  4008e5:	5e                   	pop    %rsi
  4008e6:	48 89 e2             	mov    %rsp,%rdx
  4008e9:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  4008ed:	50                   	push   %rax
  4008ee:	54                   	push   %rsp
  4008ef:	49 c7 c0 90 0c 40 00 	mov    $0x400c90,%r8
  4008f6:	48 c7 c1 20 0c 40 00 	mov    $0x400c20,%rcx
  4008fd:	48 c7 c7 cd 09 40 00 	mov    $0x4009cd,%rdi
  400904:	e8 47 ff ff ff       	callq  400850 <__libc_start_main@plt>
  400909:	f4                   	hlt    
  40090a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000400910 <deregister_tm_clones>:
  400910:	b8 8f 20 60 00       	mov    $0x60208f,%eax
  400915:	55                   	push   %rbp
  400916:	48 2d 88 20 60 00    	sub    $0x602088,%rax
  40091c:	48 83 f8 0e          	cmp    $0xe,%rax
  400920:	48 89 e5             	mov    %rsp,%rbp
  400923:	77 02                	ja     400927 <deregister_tm_clones+0x17>
  400925:	5d                   	pop    %rbp
  400926:	c3                   	retq   
  400927:	b8 00 00 00 00       	mov    $0x0,%eax
  40092c:	48 85 c0             	test   %rax,%rax
  40092f:	74 f4                	je     400925 <deregister_tm_clones+0x15>
  400931:	5d                   	pop    %rbp
  400932:	bf 88 20 60 00       	mov    $0x602088,%edi
  400937:	ff e0                	jmpq   *%rax
  400939:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000400940 <register_tm_clones>:
  400940:	b8 88 20 60 00       	mov    $0x602088,%eax
  400945:	55                   	push   %rbp
  400946:	48 2d 88 20 60 00    	sub    $0x602088,%rax
  40094c:	48 c1 f8 03          	sar    $0x3,%rax
  400950:	48 89 e5             	mov    %rsp,%rbp
  400953:	48 89 c2             	mov    %rax,%rdx
  400956:	48 c1 ea 3f          	shr    $0x3f,%rdx
  40095a:	48 01 d0             	add    %rdx,%rax
  40095d:	48 d1 f8             	sar    %rax
  400960:	75 02                	jne    400964 <register_tm_clones+0x24>
  400962:	5d                   	pop    %rbp
  400963:	c3                   	retq   
  400964:	ba 00 00 00 00       	mov    $0x0,%edx
  400969:	48 85 d2             	test   %rdx,%rdx
  40096c:	74 f4                	je     400962 <register_tm_clones+0x22>
  40096e:	5d                   	pop    %rbp
  40096f:	48 89 c6             	mov    %rax,%rsi
  400972:	bf 88 20 60 00       	mov    $0x602088,%edi
  400977:	ff e2                	jmpq   *%rdx
  400979:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000400980 <__do_global_dtors_aux>:
  400980:	80 3d 35 17 20 00 00 	cmpb   $0x0,0x201735(%rip)        # 6020bc <completed.6355>
  400987:	75 11                	jne    40099a <__do_global_dtors_aux+0x1a>
  400989:	55                   	push   %rbp
  40098a:	48 89 e5             	mov    %rsp,%rbp
  40098d:	e8 7e ff ff ff       	callq  400910 <deregister_tm_clones>
  400992:	5d                   	pop    %rbp
  400993:	c6 05 22 17 20 00 01 	movb   $0x1,0x201722(%rip)        # 6020bc <completed.6355>
  40099a:	f3 c3                	repz retq 
  40099c:	0f 1f 40 00          	nopl   0x0(%rax)

00000000004009a0 <frame_dummy>:
  4009a0:	48 83 3d 48 14 20 00 	cmpq   $0x0,0x201448(%rip)        # 601df0 <__JCR_END__>
  4009a7:	00 
  4009a8:	74 1e                	je     4009c8 <frame_dummy+0x28>
  4009aa:	b8 00 00 00 00       	mov    $0x0,%eax
  4009af:	48 85 c0             	test   %rax,%rax
  4009b2:	74 14                	je     4009c8 <frame_dummy+0x28>
  4009b4:	55                   	push   %rbp
  4009b5:	bf f0 1d 60 00       	mov    $0x601df0,%edi
  4009ba:	48 89 e5             	mov    %rsp,%rbp
  4009bd:	ff d0                	callq  *%rax
  4009bf:	5d                   	pop    %rbp
  4009c0:	e9 7b ff ff ff       	jmpq   400940 <register_tm_clones>
  4009c5:	0f 1f 00             	nopl   (%rax)
  4009c8:	e9 73 ff ff ff       	jmpq   400940 <register_tm_clones>

00000000004009cd <main>:
  4009cd:	55                   	push   %rbp
  4009ce:	48 89 e5             	mov    %rsp,%rbp
  4009d1:	48 83 ec 50          	sub    $0x50,%rsp
  4009d5:	e8 56 fe ff ff       	callq  400830 <initscr@plt>
  4009da:	48 8b 05 cf 16 20 00 	mov    0x2016cf(%rip),%rax        # 6020b0 <stdscr>
  4009e1:	48 89 c7             	mov    %rax,%rdi
  4009e4:	e8 a7 fe ff ff       	callq  400890 <wclear@plt>
  4009e9:	e8 b2 fe ff ff       	callq  4008a0 <noecho@plt>
  4009ee:	bf 00 00 00 00       	mov    $0x0,%edi
  4009f3:	e8 d8 fe ff ff       	callq  4008d0 <curs_set@plt>
  4009f8:	8b 05 a2 16 20 00    	mov    0x2016a2(%rip),%eax        # 6020a0 <COLS>
  4009fe:	89 45 f4             	mov    %eax,-0xc(%rbp)
  400a01:	8b 05 b1 16 20 00    	mov    0x2016b1(%rip),%eax        # 6020b8 <LINES>
  400a07:	89 45 f0             	mov    %eax,-0x10(%rbp)
  400a0a:	48 b8 00 00 00 00 00 	movabs $0x4034000000000000,%rax
  400a11:	00 34 40 
  400a14:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
  400a18:	48 b8 00 00 00 00 00 	movabs $0x4020000000000000,%rax
  400a1f:	00 20 40 
  400a22:	48 89 45 e0          	mov    %rax,-0x20(%rbp)
  400a26:	b8 00 00 00 00       	mov    $0x0,%eax
  400a2b:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
  400a2f:	b8 00 00 00 00       	mov    $0x0,%eax
  400a34:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
  400a38:	e9 9c 01 00 00       	jmpq   400bd9 <main+0x20c>
  400a3d:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  400a41:	48 89 45 b8          	mov    %rax,-0x48(%rbp)
  400a45:	f2 0f 10 45 b8       	movsd  -0x48(%rbp),%xmm0
  400a4a:	e8 61 fe ff ff       	callq  4008b0 <sin@plt>
  400a4f:	f2 0f 59 45 e8       	mulsd  -0x18(%rbp),%xmm0
  400a54:	f2 0f 11 45 b8       	movsd  %xmm0,-0x48(%rbp)
  400a59:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  400a5d:	48 89 45 b0          	mov    %rax,-0x50(%rbp)
  400a61:	f2 0f 10 45 b0       	movsd  -0x50(%rbp),%xmm0
  400a66:	e8 45 fe ff ff       	callq  4008b0 <sin@plt>
  400a6b:	66 0f 28 d8          	movapd %xmm0,%xmm3
  400a6f:	f2 0f 59 5d b8       	mulsd  -0x48(%rbp),%xmm3
  400a74:	f2 0f 11 5d b8       	movsd  %xmm3,-0x48(%rbp)
  400a79:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  400a7d:	48 89 45 b0          	mov    %rax,-0x50(%rbp)
  400a81:	f2 0f 10 45 b0       	movsd  -0x50(%rbp),%xmm0
  400a86:	e8 25 fe ff ff       	callq  4008b0 <sin@plt>
  400a8b:	f2 0f 59 45 b8       	mulsd  -0x48(%rbp),%xmm0
  400a90:	f2 0f 11 45 d8       	movsd  %xmm0,-0x28(%rbp)
  400a95:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  400a99:	48 89 45 b8          	mov    %rax,-0x48(%rbp)
  400a9d:	f2 0f 10 45 b8       	movsd  -0x48(%rbp),%xmm0
  400aa2:	e8 99 fd ff ff       	callq  400840 <cos@plt>
  400aa7:	f2 0f 11 45 b8       	movsd  %xmm0,-0x48(%rbp)
  400aac:	f2 0f 10 45 f8       	movsd  -0x8(%rbp),%xmm0
  400ab1:	f2 0f 58 c0          	addsd  %xmm0,%xmm0
  400ab5:	e8 86 fd ff ff       	callq  400840 <cos@plt>
  400aba:	f2 0f 10 0d ee 01 00 	movsd  0x1ee(%rip),%xmm1        # 400cb0 <__dso_handle+0x8>
  400ac1:	00 
  400ac2:	f2 0f 59 c1          	mulsd  %xmm1,%xmm0
  400ac6:	f2 0f 10 55 b8       	movsd  -0x48(%rbp),%xmm2
  400acb:	f2 0f 5c d0          	subsd  %xmm0,%xmm2
  400acf:	f2 0f 11 55 b8       	movsd  %xmm2,-0x48(%rbp)
  400ad4:	f2 0f 10 4d f8       	movsd  -0x8(%rbp),%xmm1
  400ad9:	f2 0f 10 05 d7 01 00 	movsd  0x1d7(%rip),%xmm0        # 400cb8 <__dso_handle+0x10>
  400ae0:	00 
  400ae1:	f2 0f 59 c1          	mulsd  %xmm1,%xmm0
  400ae5:	e8 56 fd ff ff       	callq  400840 <cos@plt>
  400aea:	f2 0f 10 0d ce 01 00 	movsd  0x1ce(%rip),%xmm1        # 400cc0 <__dso_handle+0x18>
  400af1:	00 
  400af2:	f2 0f 59 c1          	mulsd  %xmm1,%xmm0
  400af6:	f2 0f 10 55 b8       	movsd  -0x48(%rbp),%xmm2
  400afb:	f2 0f 5c d0          	subsd  %xmm0,%xmm2
  400aff:	f2 0f 11 55 b8       	movsd  %xmm2,-0x48(%rbp)
  400b04:	f2 0f 10 4d f8       	movsd  -0x8(%rbp),%xmm1
  400b09:	f2 0f 10 05 b7 01 00 	movsd  0x1b7(%rip),%xmm0        # 400cc8 <__dso_handle+0x20>
  400b10:	00 
  400b11:	f2 0f 59 c1          	mulsd  %xmm1,%xmm0
  400b15:	e8 26 fd ff ff       	callq  400840 <cos@plt>
  400b1a:	f2 0f 10 0d ae 01 00 	movsd  0x1ae(%rip),%xmm1        # 400cd0 <__dso_handle+0x28>
  400b21:	00 
  400b22:	f2 0f 59 c1          	mulsd  %xmm1,%xmm0
  400b26:	f2 0f 10 55 b8       	movsd  -0x48(%rbp),%xmm2
  400b2b:	f2 0f 5c d0          	subsd  %xmm0,%xmm2
  400b2f:	66 0f 28 c2          	movapd %xmm2,%xmm0
  400b33:	f2 0f 59 45 e0       	mulsd  -0x20(%rbp),%xmm0
  400b38:	f2 0f 11 45 d0       	movsd  %xmm0,-0x30(%rbp)
  400b3d:	8b 45 f4             	mov    -0xc(%rbp),%eax
  400b40:	89 c2                	mov    %eax,%edx
  400b42:	c1 ea 1f             	shr    $0x1f,%edx
  400b45:	01 d0                	add    %edx,%eax
  400b47:	d1 f8                	sar    %eax
  400b49:	89 c2                	mov    %eax,%edx
  400b4b:	f2 0f 10 45 d8       	movsd  -0x28(%rbp),%xmm0
  400b50:	f2 0f 2c c0          	cvttsd2si %xmm0,%eax
  400b54:	01 d0                	add    %edx,%eax
  400b56:	89 45 cc             	mov    %eax,-0x34(%rbp)
  400b59:	8b 45 f0             	mov    -0x10(%rbp),%eax
  400b5c:	89 c2                	mov    %eax,%edx
  400b5e:	c1 ea 1f             	shr    $0x1f,%edx
  400b61:	01 d0                	add    %edx,%eax
  400b63:	d1 f8                	sar    %eax
  400b65:	89 c2                	mov    %eax,%edx
  400b67:	f2 0f 10 45 d0       	movsd  -0x30(%rbp),%xmm0
  400b6c:	f2 0f 2c c0          	cvttsd2si %xmm0,%eax
  400b70:	29 c2                	sub    %eax,%edx
  400b72:	89 d0                	mov    %edx,%eax
  400b74:	89 45 c8             	mov    %eax,-0x38(%rbp)
  400b77:	83 7d cc 00          	cmpl   $0x0,-0x34(%rbp)
  400b7b:	78 46                	js     400bc3 <main+0x1f6>
  400b7d:	8b 45 cc             	mov    -0x34(%rbp),%eax
  400b80:	3b 45 f4             	cmp    -0xc(%rbp),%eax
  400b83:	7d 3e                	jge    400bc3 <main+0x1f6>
  400b85:	83 7d c8 00          	cmpl   $0x0,-0x38(%rbp)
  400b89:	78 38                	js     400bc3 <main+0x1f6>
  400b8b:	8b 45 c8             	mov    -0x38(%rbp),%eax
  400b8e:	3b 45 f0             	cmp    -0x10(%rbp),%eax
  400b91:	7d 30                	jge    400bc3 <main+0x1f6>
  400b93:	48 8b 05 16 15 20 00 	mov    0x201516(%rip),%rax        # 6020b0 <stdscr>
  400b9a:	8b 55 cc             	mov    -0x34(%rbp),%edx
  400b9d:	8b 4d c8             	mov    -0x38(%rbp),%ecx
  400ba0:	89 ce                	mov    %ecx,%esi
  400ba2:	48 89 c7             	mov    %rax,%rdi
  400ba5:	e8 16 fd ff ff       	callq  4008c0 <wmove@plt>
  400baa:	83 f8 ff             	cmp    $0xffffffff,%eax
  400bad:	74 14                	je     400bc3 <main+0x1f6>
  400baf:	48 8b 05 fa 14 20 00 	mov    0x2014fa(%rip),%rax        # 6020b0 <stdscr>
  400bb6:	be 2a 00 00 00       	mov    $0x2a,%esi
  400bbb:	48 89 c7             	mov    %rax,%rdi
  400bbe:	e8 4d fc ff ff       	callq  400810 <waddch@plt>
  400bc3:	f2 0f 10 4d f8       	movsd  -0x8(%rbp),%xmm1
  400bc8:	f2 0f 10 05 08 01 00 	movsd  0x108(%rip),%xmm0        # 400cd8 <__dso_handle+0x30>
  400bcf:	00 
  400bd0:	f2 0f 58 c1          	addsd  %xmm1,%xmm0
  400bd4:	f2 0f 11 45 f8       	movsd  %xmm0,-0x8(%rbp)
  400bd9:	f2 0f 10 05 ff 00 00 	movsd  0xff(%rip),%xmm0        # 400ce0 <__dso_handle+0x38>
  400be0:	00 
  400be1:	66 0f 2e 45 f8       	ucomisd -0x8(%rbp),%xmm0
  400be6:	0f 83 51 fe ff ff    	jae    400a3d <main+0x70>
  400bec:	48 8b 05 bd 14 20 00 	mov    0x2014bd(%rip),%rax        # 6020b0 <stdscr>
  400bf3:	48 89 c7             	mov    %rax,%rdi
  400bf6:	e8 25 fc ff ff       	callq  400820 <wrefresh@plt>
  400bfb:	48 8b 05 ae 14 20 00 	mov    0x2014ae(%rip),%rax        # 6020b0 <stdscr>
  400c02:	48 89 c7             	mov    %rax,%rdi
  400c05:	e8 76 fc ff ff       	callq  400880 <wgetch@plt>
  400c0a:	e8 61 fc ff ff       	callq  400870 <endwin@plt>
  400c0f:	b8 00 00 00 00       	mov    $0x0,%eax
  400c14:	c9                   	leaveq 
  400c15:	c3                   	retq   
  400c16:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  400c1d:	00 00 00 

0000000000400c20 <__libc_csu_init>:
  400c20:	41 57                	push   %r15
  400c22:	41 89 ff             	mov    %edi,%r15d
  400c25:	41 56                	push   %r14
  400c27:	49 89 f6             	mov    %rsi,%r14
  400c2a:	41 55                	push   %r13
  400c2c:	49 89 d5             	mov    %rdx,%r13
  400c2f:	41 54                	push   %r12
  400c31:	4c 8d 25 a8 11 20 00 	lea    0x2011a8(%rip),%r12        # 601de0 <__frame_dummy_init_array_entry>
  400c38:	55                   	push   %rbp
  400c39:	48 8d 2d a8 11 20 00 	lea    0x2011a8(%rip),%rbp        # 601de8 <__init_array_end>
  400c40:	53                   	push   %rbx
  400c41:	4c 29 e5             	sub    %r12,%rbp
  400c44:	31 db                	xor    %ebx,%ebx
  400c46:	48 c1 fd 03          	sar    $0x3,%rbp
  400c4a:	48 83 ec 08          	sub    $0x8,%rsp
  400c4e:	e8 85 fb ff ff       	callq  4007d8 <_init>
  400c53:	48 85 ed             	test   %rbp,%rbp
  400c56:	74 1e                	je     400c76 <__libc_csu_init+0x56>
  400c58:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
  400c5f:	00 
  400c60:	4c 89 ea             	mov    %r13,%rdx
  400c63:	4c 89 f6             	mov    %r14,%rsi
  400c66:	44 89 ff             	mov    %r15d,%edi
  400c69:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
  400c6d:	48 83 c3 01          	add    $0x1,%rbx
  400c71:	48 39 eb             	cmp    %rbp,%rbx
  400c74:	75 ea                	jne    400c60 <__libc_csu_init+0x40>
  400c76:	48 83 c4 08          	add    $0x8,%rsp
  400c7a:	5b                   	pop    %rbx
  400c7b:	5d                   	pop    %rbp
  400c7c:	41 5c                	pop    %r12
  400c7e:	41 5d                	pop    %r13
  400c80:	41 5e                	pop    %r14
  400c82:	41 5f                	pop    %r15
  400c84:	c3                   	retq   
  400c85:	90                   	nop
  400c86:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  400c8d:	00 00 00 

0000000000400c90 <__libc_csu_fini>:
  400c90:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000400c94 <_fini>:
  400c94:	48 83 ec 08          	sub    $0x8,%rsp
  400c98:	48 83 c4 08          	add    $0x8,%rsp
  400c9c:	c3                   	retq   
