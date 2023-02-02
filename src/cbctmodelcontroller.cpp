# i n c l u d e   " c b c t m o d e l c o n t r o l l e r . h " 
 # i n c l u d e   " Q M a p " 
 # i n c l u d e   " v t k P o l y D a t a . h " 
 # i n c l u d e   " v t k S m a r t P o i n t e r . h " 
 # i n c l u d e   " v t k P o l y D a t a M a p p e r . h " 
 # i n c l u d e   " v t k A c t o r . h " 
 # i n c l u d e   " v t k O B J R e a d e r . h " 
 # i n c l u d e   " v t k G e n e r i c O p e n G L R e n d e r W i n d o w . h " 
 # i n c l u d e   " v t k P r o p e r t y . h " 
 # i n c l u d e   " v t k N a m e d C o l o r s . h " 
 # i n c l u d e   " v t k R e n d e r e r . h " 
 # i n c l u d e   " v t k T r a n s f o r m . h " 
 # i n c l u d e   " v t k A x e s A c t o r . h " 
 # i n c l u d e   " v t k C a m e r a . h " 
 # i n c l u d e   " v t k C y l i n d e r S o u r c e . h " 
 # i n c l u d e   " v t k R e n d e r e r C o l l e c t i o n . h " 
 # i n c l u d e   < v t k Q u a t e r n i o n . h > 
 # i n c l u d e   < v t k M a t r i x 4 x 4 . h > 
 # i n c l u d e   < v t k S p h e r e S o u r c e . h > 
 # i n c l u d e   < Q T h r e a d > 
 
 # i n c l u d e   < q e v e n t . h > 
 
 # i n c l u d e   " u i _ m a i n w i n d o w . h " 
 # i n c l u d e   < q d e b u g . h > 
 
 # d e f i n e   U S E _ D I S P L A Y _ G L O B A L A X I S 
 
 e n u m   d a t a T y p e 
 { 
 	 L o w e r   =   0 , 
 	 U p p e r , 
 	 P a n o , 
 	 C e p h , 
 } ; 
 e n u m   v i e w T y p e 
 { 
 	 A l l   =   0 , 
 	 M a i n , 
 	 S u b , 
 } ; 
 
 c l a s s   C B C T M o d e l C o n t r o l l e r : : I n t e r n a l   { 
 p u b l i c : 
 	 U i : : M a i n W i n d o w *   m _ p a r e n t U I   =   n u l l p t r ; 
 	 C B C T M o d e l C o n t r o l l e r *   m _ O w n e r   =   n u l l p t r ; 
 
 	 i n t   m _ M a x P a n o I m a g e   =   0 ; 
 	 i n t   m _ M a x C e p h I m a g e   =   0 ; 
 
 	 i n t   m _ c u r P o s i t i o n X   =   0 ; 
 	 i n t   m _ c u r P o s i t i o n Y   =   0 ;   / /   c u r r e n t   Y   P o s i t i o n   V a l u e 
 	 i n t   m _ c u r P o s i t i o n Z   =   0 ;   / /   c u t t e n t   Z   P o s i t i o n   V a l u e 
 
 	 d o u b l e   m _ c u r A n g l e   =   0 ; 
 	 d o u b l e *   m _ P a n o C e n t e r ; 
 
 	 b o o l   i s R u n n i n g _ P a n o   =   f a l s e ; 
 	 b o o l   i s R u n n i n g _ C e p h   =   f a l s e ; 
 	 / /   G e o m e t r y D a t a T y p e ,   v t k M a p p e r 
 	 s t r u c t   M a p p e r I t e m 
 	 { 
 	 	 Q M a p < Q S t r i n g ,   v t k S m a r t P o i n t e r < v t k P o l y D a t a M a p p e r > >   m _ m a p p e r ; 
 	 } ; 
 	 / /   G e o m e t r y D a t a T y p e ,   v t k A c t o r 
 	 s t r u c t   A c t o r I t e m 
 	 { 
 	 	 Q M a p < Q S t r i n g ,   v t k S m a r t P o i n t e r < v t k A c t o r > >   m _ a c t o r ; 
 	 } ; 
 	 / /   M e m b e r   V a r i a b l e   C o n c e a l m e n t . 
 p r i v a t e : 
 
 	 / /   G e o m e t r y D a t a T y p e ,   v t k p o l y d a t a 
 	 Q M a p   < Q S t r i n g ,   v t k S m a r t P o i n t e r < v t k P o l y D a t a > >   m _ d a t a M a p ; 
 	 / /   G e o m e t r y V i e w T y p e   ,   M a p p e r I t e m 
 	 Q M a p   < Q S t r i n g ,   M a p p e r I t e m >   m _ m a p p e r M a p ; 
 	 / /   G e o m e t r y V i e w T y p e   ,   A c t o r I t e m 
 	 Q M a p   < Q S t r i n g ,   A c t o r I t e m >   m _ a c t o r M a p ; 
 	 / /   G e o m e t r y V i e w T y p e   ,   v t k R e n d e r e r 
 	 Q M a p   < Q S t r i n g ,   v t k S m a r t P o i n t e r < v t k R e n d e r e r > >   m _ r e n d e r M a p ; 
 	 / /   G e o m e t r y V i e w T y p e   ,   v t k G e n e r i c O p e n G L R e n d e r W i n d o w 
 	 Q M a p   < Q S t r i n g ,   v t k S m a r t P o i n t e r < v t k G e n e r i c O p e n G L R e n d e r W i n d o w > >   m _ w i n d o w M a p ; 
 
 p u b l i c : 
 	 I n t e r n a l ( C B C T M o d e l C o n t r o l l e r *   o w n e r ) 
 	 	 :   m _ O w n e r ( o w n e r ) 
 	 { 
 
 
 	 } 
 
 	 / /   C B C T M o d e l C o n t r o l l e r   R e s e t   m e t h o d . 
 	 b o o l   _ i n i t i a l i z e ( )   { 
 
 	 	 / / / *   L o a d   S o u r c e   * / 
 	 	 _ l o a d _ o b j f i l e ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( L o w e r ) ,   m _ d a t a M a p ) ; 
 	 	 _ l o a d _ o b j f i l e ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( U p p e r ) ,   m _ d a t a M a p ) ; 
 	 	 _ l o a d _ o b j f i l e ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( P a n o ) ,   m _ d a t a M a p ) ; 
 	 	 _ l o a d _ o b j f i l e ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ,   m _ d a t a M a p ) ; 
 
 	 	 / / / *   C r e a t e   M a p p e r   &   A c t o r   * / 
 	 	 _ c r e a t e _ M a p p e r ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( A l l ) ,   m _ d a t a M a p ,   m _ m a p p e r M a p ) ; 
 	 	 _ c r e a t e _ M a p p e r ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( M a i n ) ,   m _ d a t a M a p ,   m _ m a p p e r M a p ) ; 
 	 	 _ c r e a t e _ M a p p e r ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ,   m _ d a t a M a p ,   m _ m a p p e r M a p ) ; 
 
 	 	 _ c r e a t e _ a c t o r ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( A l l ) ,   m _ m a p p e r M a p ,   m _ a c t o r M a p ) ; 
 	 	 _ c r e a t e _ a c t o r ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( M a i n ) ,   m _ m a p p e r M a p ,   m _ a c t o r M a p ) ; 
 	 	 _ c r e a t e _ a c t o r ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ,   m _ m a p p e r M a p ,   m _ a c t o r M a p ) ; 
 
 	 	 / / / *   C r e a t e   r e n d e r e r   &   W i n d o w   * / 
 	 	 _ c r e a t e _ r e n d e r ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( A l l ) ,   m _ a c t o r M a p ,   m _ r e n d e r M a p ) ; 
 	 	 _ c r e a t e _ r e n d e r ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( M a i n ) ,   m _ a c t o r M a p ,   m _ r e n d e r M a p ) ; 
 	 	 _ c r e a t e _ r e n d e r ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ,   m _ a c t o r M a p ,   m _ r e n d e r M a p ) ; 
 
 	 	 _ c r e a t e _ w i n d o w ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( A l l ) ,   m _ r e n d e r M a p ,   m _ w i n d o w M a p ) ; 
 	 	 _ c r e a t e _ w i n d o w ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( M a i n ) ,   m _ r e n d e r M a p ,   m _ w i n d o w M a p ) ; 
 	 	 _ c r e a t e _ w i n d o w ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ,   m _ r e n d e r M a p ,   m _ w i n d o w M a p ) ; 
 
 	 	 / /   r e g i s t a r t   w i n d o w   
 	 	 _ r e g i s t e r _ w i n d o w ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( A l l ) ,   m _ w i n d o w M a p ) ; 
 	 	 _ r e g i s t e r _ w i n d o w ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( M a i n ) ,   m _ w i n d o w M a p ) ; 
 	 	 _ r e g i s t e r _ w i n d o w ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ,   m _ w i n d o w M a p ) ; 
 
 	 	 _ u p d a t e _ r e n d e r ( ) ; 
 
 
 	 	 m _ P a n o C e n t e r   =   m _ a c t o r M a p . v a l u e ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( M a i n ) ) . m _ a c t o r . v a l u e ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( P a n o ) ) - > G e t C e n t e r ( ) ; 
 
 	 	 m _ M a x P a n o I m a g e   =   m _ p a r e n t U I - > P a n o P r o g r e s s B a r - > m a x i m u m ( ) ; 
 	 	 m _ M a x C e p h I m a g e   =   m _ p a r e n t U I - > C e p h P r o g r e s s B a r - > m a x i m u m ( ) ; 
 
 
 
 
 	 	 r e t u r n   t r u e ; 
 	 } 
 
 
 	 v o i d   _ o n _ A s c e n d i n g P u s h B u t t o n _ p r e s s e d ( )   { 
 	 	 i f   ( i s R u n n i n g _ P a n o   | |   i s R u n n i n g _ C e p h ) 
 	 	 	 r e t u r n ; 
 	 	 i f   ( m _ c u r P o s i t i o n Y   > =   0 ) 
 	 	 { 
 	 	 	 m _ c u r P o s i t i o n Y   =   0 ; 
 	 	 	 q D e b u g ( )   < <   Q S t r i n g : : f r o m L o c a l 8 B i t ( " Y   A x i s   M a x i m u m   v a l u e " ) ; 
 	 	 	 r e t u r n ; 
 	 	 } 
 	 	 e l s e   { 
 	 	 	 m _ c u r P o s i t i o n Y   + =   1 0 ; 
 
 	 	 	 v t k S m a r t P o i n t e r < v t k T r a n s f o r m >   t r a n s f o r m   =   v t k S m a r t P o i n t e r < v t k T r a n s f o r m > : : N e w ( ) ; 
 	 	 	 t r a n s f o r m - > T r a n s l a t e ( 0 ,   m _ c u r P o s i t i o n Y ,   0 ) ; 
 
 	 	 	 / /   A l l 
 	 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( U p p e r ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( A l l ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( P a n o ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( A l l ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( A l l ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 
 
 	 	 	 / /   M a i n 
 	 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( U p p e r ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( M a i n ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( P a n o ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( M a i n ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( M a i n ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 
 	 	 	 / /   S u b 
 	 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( U p p e r ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( P a n o ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 
 	 	 	 t r a n s f o r m - > U p d a t e ( ) ; 
 
 	 	 	 _ u p d a t e _ r e n d e r ( ) ; 
 	 	 } 
 
 	 } 
 
 	 v o i d   _ o n _ D e s c e n d i n g P u s h B u t t o n _ p r e s s e d ( )   { 
 	 	 i f   ( i s R u n n i n g _ P a n o   | |   i s R u n n i n g _ C e p h ) 
 	 	 	 r e t u r n ; 
 	 	 i f   ( m _ c u r P o s i t i o n Y   <   - 5 8 0 ) 
 	 	 { 
 	 	 	 m _ c u r P o s i t i o n Y   =   - 5 8 0 ; 
 	 	 	 q D e b u g ( )   < <   " Y   A x i s   M i n i m u m   v a l u e " ; 
 	 	 	 r e t u r n ; 
 	 	 } 
 	 	 e l s e   { 
 	 	 	 m _ c u r P o s i t i o n Y   - =   1 0 ; 
 	 	 	 v t k S m a r t P o i n t e r < v t k T r a n s f o r m >   t r a n s f o r m   =   v t k S m a r t P o i n t e r < v t k T r a n s f o r m > : : N e w ( ) ; 
 	 	 	 t r a n s f o r m - > T r a n s l a t e ( 0 ,   m _ c u r P o s i t i o n Y ,   0 ) ; 
 
 	 	 	 / /   A l l 
 	 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( U p p e r ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( A l l ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( P a n o ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( A l l ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( A l l ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 
 
 	 	 	 / /   M a i n 
 	 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( U p p e r ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( M a i n ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( P a n o ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( M a i n ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( M a i n ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 
 	 	 	 / /   S u b 
 	 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( U p p e r ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( P a n o ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 
 	 	 	 t r a n s f o r m - > U p d a t e ( ) ; 
 
 	 	 	 _ u p d a t e _ r e n d e r ( ) ; 
 	 	 	 q D e b u g ( )   < <   m _ c u r P o s i t i o n Y ; 
 	 	 } 
 	 } 
 
 	 v o i d   _ o n _ M a i n P u s h B u t t o n _ c l i c k e d ( )   { 
 	 	 i f   ( i s R u n n i n g _ P a n o   | |   i s R u n n i n g _ C e p h ) 
 	 	 	 r e t u r n ; 
 	 	 i s R u n n i n g _ P a n o   =   t r u e ; 
 
 	 	 f o r   ( i n t   i   =   0 ;   i   < =   3 6 0 ;   i + + ) 
 	 	 { 
 	 	 	 i f   ( ! i s R u n n i n g _ P a n o ) 
 	 	 	 	 b r e a k ; 
 
 	 	 	 _ R o t a t e _ P a n o ( i ) ; 
 	 	 } 
 	 	 i s R u n n i n g _ P a n o   =   f a l s e ; 
 	 } 
 
 	 v o i d   _ o n _ S u b P u s h B u t t o n _ c l i c k e d ( ) 
 	 { 
 	 	 i f   ( i s R u n n i n g _ P a n o   | |   i s R u n n i n g _ C e p h ) 
 	 	 	 r e t u r n ; 
 	 	 i s R u n n i n g _ C e p h   =   t r u e ; 
 
 	 	 _ A n i m a t i o n _ C e p h ( ) ; 
 
 	 	 i s R u n n i n g _ C e p h   =   f a l s e ; 
 	 } 
 
 	 v o i d   _ o n _ C a p t u r e R e s e t P u s h B u t t o n _ V T K _ c l i c k e d ( ) 
 	 { 
 	 	 i s R u n n i n g _ C e p h   =   f a l s e ; 
 	 	 i s R u n n i n g _ P a n o   =   f a l s e ; 
 
 	 	 v t k S m a r t P o i n t e r < v t k T r a n s f o r m >   t r a n s f o r m   =   v t k S m a r t P o i n t e r < v t k T r a n s f o r m > : : N e w ( ) ; 
 	 	 m _ c u r P o s i t i o n X   =   0 ; 
 	 	 m _ c u r P o s i t i o n Y   =   0 ; 
 	 	 m _ c u r P o s i t i o n Z   =   0 ; 
 	 	 t r a n s f o r m - > T r a n s l a t e ( m _ c u r P o s i t i o n X ,   m _ c u r P o s i t i o n Y ,   m _ c u r P o s i t i o n Z ) ; 
 
 	 	 / /   A l l 
 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( U p p e r ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( A l l ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( P a n o ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( A l l ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( A l l ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 
 
 	 	 / /   M a i n 
 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( U p p e r ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( M a i n ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( P a n o ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( M a i n ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( M a i n ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 
 	 	 / /   S u b 
 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( U p p e r ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( P a n o ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 
 	 	 t r a n s f o r m - > U p d a t e ( ) ; 
 
 	 	 _ u p d a t e _ r e n d e r ( ) ; 
 
 	 } 
 
 	 v o i d   _ o n _ C a p t u r e R e a d y P u s h B u t t o n _ V T K _ c l i c k e d ( ) 
 	 { 
 
 	 } 
 
 	 v o i d   _ A n i m a t i o n _ C e p h ( )   { 
 	 	 / /   m a x   2 6 0 ; 
 	 	 / /   m i n   2 7 0 ; 
 	 	 f o r   ( i n t   i   =   0 ;   i   <   5 2 ;   i + + ) 
 	 	 { 
 	 	 	 i f   ( ! i s R u n n i n g _ C e p h ) 
 	 	 	 	 b r e a k ; 
 	 	 	 m _ c u r P o s i t i o n Z   =   m _ c u r P o s i t i o n Z   +   5 ; 
 
 	 	 	 v t k S m a r t P o i n t e r < v t k T r a n s f o r m >   t r a n s f o r m   =   v t k S m a r t P o i n t e r < v t k T r a n s f o r m > : : N e w ( ) ; 
 	 	 	 t r a n s f o r m - > T r a n s l a t e ( 0 ,   m _ c u r P o s i t i o n Y ,   m _ c u r P o s i t i o n Z ) ; 
 
 	 	 	 / /   S u b 
 	 	 	 m _ a c t o r M a p . v a l u e ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ) . m _ a c t o r . v a l u e ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
                         a u t o   a c t o r   =   _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ) ; 
                         a c t o r - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 
 	 	 	 t r a n s f o r m - > U p d a t e ( ) ; 
 	 	 	 q D e b u g ( )   < <   " p o s i t i o n   Z   :   "   < <   m _ c u r P o s i t i o n Z ; 
 	 	 	 _ u p d a t e _ r e n d e r ( ) ; 
 	 	 } 
 	 	 f o r   ( i n t   i   =   0 ;   i   <   1 3 0 ;   i + + ) 
 	 	 { 
 	 	 	 i f   ( ! i s R u n n i n g _ C e p h ) 
 	 	 	 	 b r e a k ; 
 	 	 	 m _ c u r P o s i t i o n Z   =   m _ c u r P o s i t i o n Z   -   2 ; 
 
 	 	 	 v t k S m a r t P o i n t e r < v t k T r a n s f o r m >   t r a n s f o r m   =   v t k S m a r t P o i n t e r < v t k T r a n s f o r m > : : N e w ( ) ; 
 	 	 	 t r a n s f o r m - > T r a n s l a t e ( 0 ,   m _ c u r P o s i t i o n Y ,   m _ c u r P o s i t i o n Z ) ; 
 
 
 	 	 	 / /   S u b 
 	 	 	 m _ a c t o r M a p . v a l u e ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ) . m _ a c t o r . v a l u e ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
                         a u t o   a c t o r S u b   =   _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ) ; 
                         a c t o r S u b - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 
 
 	 	 	 t r a n s f o r m - > U p d a t e ( ) ; 
 	 	 	 q D e b u g ( )   < <   " p o s i t i o n   Z   :   "   < <   m _ c u r P o s i t i o n Z ; 
 	 	 	 _ u p d a t e _ r e n d e r ( ) ; 
 	 	 } 
 	 	 f o r   ( i n t   i   =   0 ;   i   <   1 3 0 ;   i + + ) 
 	 	 { 
 	 	 	 i f   ( ! i s R u n n i n g _ C e p h ) 
 	 	 	 	 b r e a k ; 
 	 	 	 m _ c u r P o s i t i o n Z   =   m _ c u r P o s i t i o n Z   -   2 ; 
 
 	 	 	 v t k S m a r t P o i n t e r < v t k T r a n s f o r m >   t r a n s f o r m   =   v t k S m a r t P o i n t e r < v t k T r a n s f o r m > : : N e w ( ) ; 
 	 	 	 t r a n s f o r m - > T r a n s l a t e ( 0 ,   m _ c u r P o s i t i o n Y ,   m _ c u r P o s i t i o n Z ) ; 
 
 
 	 	 	 / /   S u b 
 	 	 	 m _ a c t o r M a p . v a l u e ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ) . m _ a c t o r . v a l u e ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 	 a u t o   a c t o r   =   _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ) ; 
 
 	 	 	 a c t o r - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 
 
 	 	 	 t r a n s f o r m - > U p d a t e ( ) ; 
 	 	 	 q D e b u g ( )   < <   " p o s i t i o n   Z   :   "   < <   m _ c u r P o s i t i o n Z ; 
 	 	 	 _ u p d a t e _ r e n d e r ( ) ; 
 	 	 } 
 	 	 f o r   ( i n t   i   =   0 ;   i   <   5 2 ;   i + + ) 
 	 	 { 
 	 	 	 i f   ( ! i s R u n n i n g _ C e p h ) 
 	 	 	 	 b r e a k ; 
 	 	 	 m _ c u r P o s i t i o n Z   =   m _ c u r P o s i t i o n Z   +   5 ; 
 
 	 	 	 v t k S m a r t P o i n t e r < v t k T r a n s f o r m >   t r a n s f o r m   =   v t k S m a r t P o i n t e r < v t k T r a n s f o r m > : : N e w ( ) ; 
 	 	 	 t r a n s f o r m - > T r a n s l a t e ( 0 ,   m _ c u r P o s i t i o n Y ,   m _ c u r P o s i t i o n Z ) ; 
 
 	 	 	 / /   S u b 
 	 	 	 m _ a c t o r M a p . v a l u e ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ) . m _ a c t o r . v a l u e ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 	 a u t o   a c t o r   =   _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ) ; 
 	 	 	 a c t o r - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 	 t r a n s f o r m - > U p d a t e ( ) ; 
 
 	 	 	 q D e b u g ( )   < <   " p o s i t i o n   Z   :   "   < <   m _ c u r P o s i t i o n Z ; 
 	 	 	 _ u p d a t e _ r e n d e r ( ) ; 
 	 	 } 
 	 } 
 
 	 v o i d   _ R o t a t e _ P a n o ( i n t   a n g l e ) 
 	 { 
 	 	 v t k S m a r t P o i n t e r < v t k T r a n s f o r m >   t r a n s f o r m   =   v t k S m a r t P o i n t e r < v t k T r a n s f o r m > : : N e w ( ) ; 
 	 	 t r a n s f o r m - > P o s t M u l t i p l y ( ) ; 
 	 	 t r a n s f o r m - > T r a n s l a t e ( - m _ P a n o C e n t e r [ 0 ] ,   - m _ P a n o C e n t e r [ 1 ] ,   - m _ P a n o C e n t e r [ 2 ] ) ; 
 	 	 t r a n s f o r m - > R o t a t e Y ( a n g l e ) ; 
 	 	 t r a n s f o r m - > T r a n s l a t e ( m _ P a n o C e n t e r ) ; 
 	 	 t r a n s f o r m - > T r a n s l a t e ( 0 ,   m _ c u r P o s i t i o n Y ,   0 ) ; 
 
 	 	 m _ a c t o r M a p . v a l u e ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( M a i n ) ) . m _ a c t o r . v a l u e ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( P a n o ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
                 a u t o   a c t o r M a i n   =   _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( P a n o ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( M a i n ) ) ; 
                 a c t o r M a i n - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 t r a n s f o r m - > U p d a t e ( ) ; 
 
                 m _ a c t o r M a p . v a l u e ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( A l l ) ) . m _ a c t o r . v a l u e ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( P a n o ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
                 a u t o   a c t o r A l l   =   _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( P a n o ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( A l l ) ) ; 
                 a c t o r A l l - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
                 t r a n s f o r m - > U p d a t e ( ) ; 
 
 	 	 q D e b u g ( )   < <   a n g l e ; 
 	 	 _ u p d a t e _ r e n d e r ( ) ; 
 	 } 
 
 	 v o i d   _ T r a n s l a t e _ C e p h ( d o u b l e   p o s Z ) 
 	 { 
 	 	 v t k S m a r t P o i n t e r < v t k T r a n s f o r m >   t r a n s f o r m   =   v t k S m a r t P o i n t e r < v t k T r a n s f o r m > : : N e w ( ) ; 
 	 	 t r a n s f o r m - > T r a n s l a t e ( 0 ,   m _ c u r P o s i t i o n Y ,   p o s Z ) ; 
 
 	 	 / /   S u b 
 	 	 m _ a c t o r M a p . v a l u e ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ) . m _ a c t o r . v a l u e ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
                 a u t o   a c t o r S u b   =   _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( S u b ) ) ; 
                 a c t o r S u b - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
 	 	 t r a n s f o r m - > U p d a t e ( ) ; 
 
                 m _ a c t o r M a p . v a l u e ( G e o m e t r y V i e w T y p e : : t o S t r i n g ( A l l ) ) . m _ a c t o r . v a l u e ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ) - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
                 a u t o   a c t o r A l l   =   _ g e t _ a c t o r ( G e o m e t r y D a t a T y p e : : t o S t r i n g ( C e p h ) ,   G e o m e t r y V i e w T y p e : : t o S t r i n g ( A l l ) ) ; 
                 a c t o r A l l - > S e t U s e r T r a n s f o r m ( t r a n s f o r m ) ; 
                 t r a n s f o r m - > U p d a t e ( ) ; 
 
                 q D e b u g ( )   < <   " p o s i t i o n   Z   :   "   < <   p o s Z ; 
 	 	 _ u p d a t e _ r e n d e r ( ) ; 
 	 } 
 	 / /   I n t e r n a l   M e t h o d s   m u s t   b e   u s e d   i n   t h e   I n t e r n a l . 
 p r i v a t e : 
 	 v o i d   _ l o a d _ o b j f i l e ( c o n s t   Q S t r i n g &   t y p e ,   Q M a p < Q S t r i n g ,   v t k S m a r t P o i n t e r < v t k P o l y D a t a > > &   o b j s ) 
 	 { 
 	 	 v t k S m a r t P o i n t e r < v t k O B J R e a d e r >   r e a d e r   =   v t k S m a r t P o i n t e r < v t k O B J R e a d e r > : : N e w ( ) ; 
 	 	 s w i t c h   ( G e o m e t r y D a t a T y p e : : t o E n u m ( t y p e ) ) 
 	 	 { 
 	 	 c a s e   G e o m e t r y D a t a T y p e : : L o w e r : 
 	 	 { 
 	 	 	 r e a d e r - > S e t F i l e N a m e ( " l o w e r b o d y . o b j " ) ; 
 
 	 	 } b r e a k ; 
 	 	 c a s e   G e o m e t r y D a t a T y p e : : U p p e r : 
 	 	 { 
 	 	 	 r e a d e r - > S e t F i l e N a m e ( " u p p e r b o d y . o b j " ) ; 
 	 	 } b r e a k ; 
 	 	 c a s e   G e o m e t r y D a t a T y p e : : P a n o r a m a : 
 	 	 { 
 	 	 	 r e a d e r - > S e t F i l e N a m e ( " p a n o m o d u l e . o b j " ) ; 
 	 	 } b r e a k ; 
 	 	 c a s e   G e o m e t r y D a t a T y p e : : C e p h a l o : 
 	 	 { 
 	 	 	 r e a d e r - > S e t F i l e N a m e ( " c e p h m o d u l e . o b j " ) ; 
 	 	 } b r e a k ; 
 	 	 } 
 	 	 r e a d e r - > U p d a t e ( ) ; 
 	 	 o b j s . i n s e r t ( t y p e ,   r e a d e r - > G e t O u t p u t ( ) ) ; 
 	 } 
 
 	 v o i d   _ c r e a t e _ M a p p e r ( c o n s t   Q S t r i n g &   v i e w T y p e ,   Q M a p < Q S t r i n g ,   v t k S m a r t P o i n t e r < v t k P o l y D a t a > > &   o b j s ,   Q M a p < Q S t r i n g ,   M a p p e r I t e m > &   m a p p e r M a p ) 
 	 { 
 
 	 	 s w i t c h   ( G e o m e t r y V i e w T y p e : : t o E n u m ( v i e w T y p e ) ) 
 	 	 { 
 	 	 c a s e   G e o m e t r y V i e w T y p e : : A l l : 
 	 	 { 
 	 	 	 M a p p e r I t e m   i t e m ; 
 	 	 	 f o r   ( a u t o   i t   =   o b j s . k e y B e g i n ( ) ;   i t   ! =   o b j s . k e y E n d ( ) ;   i t + + ) 
 	 	 	 { 
 	 	 	 	 v t k S m a r t P o i n t e r < v t k P o l y D a t a M a p p e r >   m a p   =   v t k S m a r t P o i n t e r < v t k P o l y D a t a M a p p e r > : : N e w ( ) ; 
 	 	 	 	 m a p - > S e t I n p u t D a t a ( o b j s . v a l u e ( * i t ) ) ; 
 	 	 	 	 m a p - > U p d a t e ( ) ; 
 	 	 	 	 i t e m . m _ m a p p e r . i n s e r t ( * i t ,   m a p ) ; 
 	 	 	 } 
 	 	 	 m a p p e r M a p . i n s e r t ( v i e w T y p e ,   i t e m ) ; 
 	 	 } b r e a k ; 
 	 	 c a s e   G e o m e t r y V i e w T y p e : : M a i n : 
 	 	 { 
 	 	 	 M a p p e r I t e m   i t e m ; 
 	 	 	 f o r   ( a u t o   i t   =   o b j s . k e y B e g i n ( ) ;   i t   ! =   o b j s . k e y E n d ( ) ;   i t + + ) 
 	 	 	 { 
 	 	 	 	 v t k S m a r t P o i n t e r < v t k P o l y D a t a M a p p e r >   m a p   =   v t k S m a r t P o i n t e r < v t k P o l y D a t a M a p p e r > : : N e w ( ) ; 
 	 	 	 	 m a p - > S e t I n p u t D a t a ( o b j s . v a l u e ( * i t ) ) ; 
 	 	 	 	 m a p - > U p d a t e ( ) ; 
 	 	 	 	 i t e m . m _ m a p p e r . i n s e r t ( * i t ,   m a p ) ; 
 	 	 	 } 
 	 	 	 m a p p e r M a p . i n s e r t ( v i e w T y p e ,   i t e m ) ; 
 	 	 } b r e a k ; 
 	 	 c a s e   G e o m e t r y V i e w T y p e : : S u b : 
 	 	 { 
 	 	 	 M a p p e r I t e m   i t e m ; 
 	 	 	 f o r   ( a u t o   i t   =   o b j s . k e y B e g i n ( ) ;   i t   ! =   o b j s . k e y E n d ( ) ;   i t + + ) 
 	 	 	 { 
 	 	 	 	 v t k S m a r t P o i n t e r < v t k P o l y D a t a M a p p e r >   m a p   =   v t k S m a r t P o i n t e r < v t k P o l y D a t a M a p p e r > : : N e w ( ) ; 
 	 	 	 	 m a p - > S e t I n p u t D a t a ( o b j s . v a l u e ( * i t ) ) ; 
 	 	 	 	 m a p - > U p d a t e ( ) ; 
 	 	 	 	 i t e m . m _ m a p p e r . i n s e r t ( * i t ,   m a p ) ; 
 	 	 	 } 
 	 	 	 m a p p e r M a p . i n s e r t ( v i e w T y p e ,   i t e m ) ; 
 	 	 } b r e a k ; 
 	 	 d e f a u l t : 
 	 	 	 b r e a k ; 
 	 	 } 
 	 } 
 
 	 v o i d   _ c r e a t e _ a c t o r ( c o n s t   Q S t r i n g &   v i e w T y p e ,   Q M a p < Q S t r i n g ,   M a p p e r I t e m > &   m a p p e r M a p ,   Q M a p < Q S t r i n g ,   A c t o r I t e m > &   a c t o r M a p ) 
 	 { 
 
 	 	 v t k S m a r t P o i n t e r < v t k A c t o r >   a c t o r   =   v t k S m a r t P o i n t e r < v t k A c t o r > : : N e w ( ) ; 
 	 	 s w i t c h   ( G e o m e t r y V i e w T y p e : : t o E n u m ( v i e w T y p e ) ) 
 	 	 { 
 	 	 c a s e   G e o m e t r y V i e w T y p e : : A l l : 
 	 	 { 
 	 	 	 i f   ( m a p p e r M a p . c o n t a i n s ( v i e w T y p e ) ) 
 	 	 	 { 
 	 	 	 	 a u t o   m a p   =   m a p p e r M a p . v a l u e ( v i e w T y p e ) . m _ m a p p e r ; 
 	 	 	 	 A c t o r I t e m   i t e m ; 
 	 	 	 	 f o r   ( a u t o   i t   =   m a p . k e y B e g i n ( ) ;   i t   ! =   m a p . k e y E n d ( ) ;   i t + + ) 
 	 	 	 	 { 
 	 	 	 	 	 v t k S m a r t P o i n t e r < v t k A c t o r >   a c t o r   =   v t k S m a r t P o i n t e r < v t k A c t o r > : : N e w ( ) ; 
 
 	 	 	 	 	 a c t o r - > S e t M a p p e r ( m a p . v a l u e ( * i t ) ) ; 
 	 	 	 	 	 i t e m . m _ a c t o r . i n s e r t ( * i t ,   a c t o r ) ; 
 	 	 	 	 } 
 
 	 	 	 	 a c t o r M a p . i n s e r t ( v i e w T y p e ,   i t e m ) ; 
 	 	 	 	 q D e b u g ( )   < <   " a c t o r   S i z e   :   "   < <   i t e m . m _ a c t o r . s i z e ( ) ; 
 	 	 	 	 q D e b u g ( )   < <   " a c t o r   S i z e   :   "   < <   a c t o r M a p . v a l u e ( v i e w T y p e ) . m _ a c t o r . s i z e ( ) ; 
 
 	 	 	 } 
 	 	 } b r e a k ; 
 	 	 c a s e   G e o m e t r y V i e w T y p e : : M a i n : 
 	 	 { 
 	 	 	 i f   ( m a p p e r M a p . c o n t a i n s ( v i e w T y p e ) ) 
 	 	 	 { 
 	 	 	 	 a u t o   m a p   =   m a p p e r M a p . v a l u e ( v i e w T y p e ) . m _ m a p p e r ; 
 	 	 	 	 A c t o r I t e m   i t e m ; 
 	 	 	 	 f o r   ( a u t o   i t   =   m a p . k e y B e g i n ( ) ;   i t   ! =   m a p . k e y E n d ( ) ;   i t + + ) 
 	 	 	 	 { 
 	 	 	 	 	 v t k S m a r t P o i n t e r < v t k A c t o r >   a c t o r   =   v t k S m a r t P o i n t e r < v t k A c t o r > : : N e w ( ) ; 
 	 	 	 	 	 a c t o r - > S e t M a p p e r ( m a p . v a l u e ( * i t ) ) ; 
 	 	 	 	 	 i t e m . m _ a c t o r . i n s e r t ( * i t ,   a c t o r ) ; 
 	 	 	 	 } 
 	 	 	 	 a c t o r M a p . i n s e r t ( v i e w T y p e ,   i t e m ) ; 
 	 	 	 } 
 	 	 } b r e a k ; 
 	 	 c a s e   G e o m e t r y V i e w T y p e : : S u b : 
 	 	 { 
 	 	 	 i f   ( m a p p e r M a p . c o n t a i n s ( v i e w T y p e ) ) 
 	 	 	 { 
 	 	 	 	 a u t o   m a p   =   m a p p e r M a p . v a l u e ( v i e w T y p e ) . m _ m a p p e r ; 
 	 	 	 	 A c t o r I t e m   i t e m ; 
 	 	 	 	 f o r   ( a u t o   i t   =   m a p . k e y B e g i n ( ) ;   i t   ! =   m a p . k e y E n d ( ) ;   i t + + ) 
 	 	 	 	 { 
 	 	 	 	 	 v t k S m a r t P o i n t e r < v t k A c t o r >   a c t o r   =   v t k S m a r t P o i n t e r < v t k A c t o r > : : N e w ( ) ; 
 
 	 	 	 	 	 a c t o r - > S e t M a p p e r ( m a p . v a l u e ( * i t ) ) ; 
 	 	 	 	 	 i t e m . m _ a c t o r . i n s e r t ( * i t ,   a c t o r ) ; 
 	 	 	 	 } 
 	 	 	 	 a c t o r M a p . i n s e r t ( v i e w T y p e ,   i t e m ) ; 
 	 	 	 } 
 	 	 } b r e a k ; 
 	 	 d e f a u l t : 
 	 	 	 b r e a k ; 
 	 	 } 
 	 } 
 
 	 v o i d   _ c r e a t e _ r e n d e r ( c o n s t   Q S t r i n g &   v i e w T y p e ,   Q M a p < Q S t r i n g ,   A c t o r I t e m > &   a c t o r M a p ,   Q M a p < Q S t r i n g ,   v t k S m a r t P o i n t e r < v t k R e n d e r e r > > &   r e n d e r M a p ) 
 	 { 
 	 	 v t k S m a r t P o i n t e r < v t k N a m e d C o l o r s >   c o l o r s   =   v t k S m a r t P o i n t e r < v t k N a m e d C o l o r s > : : N e w ( ) ; 
 	 	 v t k S m a r t P o i n t e r < v t k R e n d e r e r >   R e n d e r e r   =   v t k S m a r t P o i n t e r < v t k R e n d e r e r > : : N e w ( ) ; 
 
 # i f d e f   U S E _ D I S P L A Y _ G L O B A L A X I S 
 	 	 v t k S m a r t P o i n t e r < v t k A x e s A c t o r >   a x e s   =   v t k S m a r t P o i n t e r < v t k A x e s A c t o r > : : N e w ( ) ; 
 	 	 a x e s - > S e t T o t a l L e n g t h ( 7 0 0 ,   7 0 0 ,   7 0 0 ) ; 
 	 	 a u t o   c e n   =   a x e s - > G e t C e n t e r ( ) ; 
 	 	 R e n d e r e r - > A d d A c t o r ( a x e s ) ; 
 # e n d i f 
 	 	 s w i t c h   ( G e o m e t r y V i e w T y p e : : t o E n u m ( v i e w T y p e ) ) 
 	 	 { 
 	 	 c a s e   G e o m e t r y V i e w T y p e : : A l l : 
 	 	 { 
 	 	 	 i f   ( a c t o r M a p . c o n t a i n s ( v i e w T y p e ) ) 
 	 	 	 { 
 	 	 	 	 a u t o   a c t o r   =   a c t o r M a p . v a l u e ( v i e w T y p e ) . m _ a c t o r ; 
 	 	 	 	 f o r   ( a u t o   i t   =   a c t o r . k e y B e g i n ( ) ;   i t   ! =   a c t o r . k e y E n d ( ) ;   i t + + ) 
 	 	 	 	 { 
 	 	 	 	 	 R e n d e r e r - > A d d A c t o r ( a c t o r . v a l u e ( * i t ) ) ; 
 	 	 	 	 } 
 	 	 	 	 R e n d e r e r - > S e t B a c k g r o u n d ( c o l o r s - > G e t C o l o r 3 d ( " B l a c k " ) . G e t D a t a ( ) ) ; 
 	 	 	 	 R e n d e r e r - > R e s e t C a m e r a ( ) ; 
 
 	 	 	 	 r e n d e r M a p . i n s e r t ( v i e w T y p e ,   R e n d e r e r ) ; 
 	 	 	 } 
 	 	 } b r e a k ; 
 	 	 c a s e   G e o m e t r y V i e w T y p e : : M a i n : 
 	 	 { 
 	 	 	 i f   ( a c t o r M a p . c o n t a i n s ( v i e w T y p e ) ) 
 	 	 	 { 
 	 	 	 	 a u t o   a c t o r   =   a c t o r M a p . v a l u e ( v i e w T y p e ) . m _ a c t o r ; 
 	 	 	 	 f o r   ( a u t o   i t   =   a c t o r . k e y B e g i n ( ) ;   i t   ! =   a c t o r . k e y E n d ( ) ;   i t + + ) 
 	 	 	 	 { 
 	 	 	 	 	 R e n d e r e r - > A d d A c t o r ( a c t o r . v a l u e ( * i t ) ) ; 
 	 	 	 	 } 
 	 	 	 	 R e n d e r e r - > S e t B a c k g r o u n d ( c o l o r s - > G e t C o l o r 3 d ( " B l a c k " ) . G e t D a t a ( ) ) ; 
 	 	 	 	 R e n d e r e r - > R e s e t C a m e r a ( ) ; 
 
 	 	 	 	 r e n d e r M a p . i n s e r t ( v i e w T y p e ,   R e n d e r e r ) ; 
 	 	 	 } 
 	 	 } b r e a k ; 
 	 	 c a s e   G e o m e t r y V i e w T y p e : : S u b : 
 	 	 { 
 	 	 	 i f   ( a c t o r M a p . c o n t a i n s ( v i e w T y p e ) ) 
 	 	 	 { 
 	 	 	 	 a u t o   a c t o r   =   a c t o r M a p . v a l u e ( v i e w T y p e ) . m _ a c t o r ; 
 	 	 	 	 f o r   ( a u t o   i t   =   a c t o r . k e y B e g i n ( ) ;   i t   ! =   a c t o r . k e y E n d ( ) ;   i t + + ) 
 	 	 	 	 { 
 	 	 	 	 	 R e n d e r e r - > A d d A c t o r ( a c t o r . v a l u e ( * i t ) ) ; 
 	 	 	 	 } 
 	 	 	 	 R e n d e r e r - > S e t B a c k g r o u n d ( c o l o r s - > G e t C o l o r 3 d ( " B l a c k " ) . G e t D a t a ( ) ) ; 
 	 	 	 	 R e n d e r e r - > R e s e t C a m e r a ( ) ; 
 
 	 	 	 	 r e n d e r M a p . i n s e r t ( v i e w T y p e ,   R e n d e r e r ) ; 
 	 	 	 } 
 	 	 } b r e a k ; 
 	 	 d e f a u l t : 
 	 	 	 b r e a k ; 
 	 	 } 
 	 } 
 
 	 v o i d   _ c r e a t e _ w i n d o w ( c o n s t   Q S t r i n g &   v i e w T y p e ,   Q M a p < Q S t r i n g ,   v t k S m a r t P o i n t e r < v t k R e n d e r e r > > &   r e n d e r M a p ,   Q M a p < Q S t r i n g ,   v t k S m a r t P o i n t e r < v t k G e n e r i c O p e n G L R e n d e r W i n d o w > > &   w i n d o w M a p ) 
 	 { 
 	 	 v t k S m a r t P o i n t e r < v t k G e n e r i c O p e n G L R e n d e r W i n d o w >   r e n d e r W i n d o w   =   v t k S m a r t P o i n t e r < v t k G e n e r i c O p e n G L R e n d e r W i n d o w > : : N e w ( ) ; 
 
 	 	 s w i t c h   ( G e o m e t r y V i e w T y p e : : t o E n u m ( v i e w T y p e ) ) 
 	 	 { 
 	 	 c a s e   G e o m e t r y V i e w T y p e : : A l l : 
 	 	 { 
 	 	 	 i f   ( r e n d e r M a p . c o n t a i n s ( v i e w T y p e ) ) 
 	 	 	 { 
 	 	 	 	 a u t o   r e n d e r   =   r e n d e r M a p . v a l u e ( v i e w T y p e ) ; 
 	 	 	 	 r e n d e r W i n d o w - > A d d R e n d e r e r ( r e n d e r ) ; 
 	 	 	 	 w i n d o w M a p . i n s e r t ( v i e w T y p e ,   r e n d e r W i n d o w ) ; 
 	 	 	 } 
 	 	 } b r e a k ; 
 	 	 c a s e   G e o m e t r y V i e w T y p e : : M a i n : 
 	 	 { 
 	 	 	 i f   ( r e n d e r M a p . c o n t a i n s ( v i e w T y p e ) ) 
 	 	 	 { 
 	 	 	 	 a u t o   r e n d e r   =   r e n d e r M a p . v a l u e ( v i e w T y p e ) ; 
 	 	 	 	 r e n d e r W i n d o w - > A d d R e n d e r e r ( r e n d e r ) ; 
 	 	 	 	 w i n d o w M a p . i n s e r t ( v i e w T y p e ,   r e n d e r W i n d o w ) ; 
 	 	 	 } 
 	 	 } b r e a k ; 
 	 	 c a s e   G e o m e t r y V i e w T y p e : : S u b : 
 	 	 { 
 	 	 	 i f   ( r e n d e r M a p . c o n t a i n s ( v i e w T y p e ) ) 
 	 	 	 { 
 	 	 	 	 a u t o   r e n d e r   =   r e n d e r M a p . v a l u e ( v i e w T y p e ) ; 
 	 	 	 	 r e n d e r W i n d o w - > A d d R e n d e r e r ( r e n d e r ) ; 
 	 	 	 	 w i n d o w M a p . i n s e r t ( v i e w T y p e ,   r e n d e r W i n d o w ) ; 
 	 	 	 } 
 	 	 } b r e a k ; 
 	 	 d e f a u l t : 
 	 	 	 b r e a k ; 
 	 	 } 
 	 } 
 
 	 v o i d   _ r e g i s t e r _ w i n d o w ( c o n s t   Q S t r i n g &   v i e w T y p e ,   Q M a p < Q S t r i n g ,   v t k S m a r t P o i n t e r < v t k G e n e r i c O p e n G L R e n d e r W i n d o w > > &   w i n d o w M a p ) 
 	 { 
 	 	 s w i t c h   ( G e o m e t r y V i e w T y p e : : t o E n u m ( v i e w T y p e ) ) 
 	 	 { 
 	 	 c a s e   G e o m e t r y V i e w T y p e : : A l l : 
 	 	 { 
 	 	 	 m _ p a r e n t U I - > o p e n G L W i d g e t _ A l l - > s e t R e n d e r W i n d o w ( w i n d o w M a p . v a l u e ( v i e w T y p e ) ) ; 
 	 	 	 m _ p a r e n t U I - > o p e n G L W i d g e t _ A l l - > i n t e r a c t o r ( ) - > P r o c e s s E v e n t s ( ) ; 
 
 	 	 } b r e a k ; 
 	 	 c a s e   G e o m e t r y V i e w T y p e : : M a i n : 
 	 	 { 
 	 	 	 m _ p a r e n t U I - > o p e n G L W i d g e t _ M a i n - > s e t R e n d e r W i n d o w ( w i n d o w M a p . v a l u e ( v i e w T y p e ) ) ; 
 	 	 	 m _ p a r e n t U I - > o p e n G L W i d g e t _ M a i n - > i n t e r a c t o r ( ) - > P r o c e s s E v e n t s ( ) ; 
 
 	 	 } b r e a k ; 
 	 	 c a s e   G e o m e t r y V i e w T y p e : : S u b : 
 	 	 { 
 	 	 	 m _ p a r e n t U I - > o p e n G L W i d g e t _ S u b - > s e t R e n d e r W i n d o w ( w i n d o w M a p . v a l u e ( v i e w T y p e ) ) ; 
 	 	 	 m _ p a r e n t U I - > o p e n G L W i d g e t _ S u b - > i n t e r a c t o r ( ) - > P r o c e s s E v e n t s ( ) ; 
 	 	 } b r e a k ; 
 	 	 d e f a u l t : 
 	 	 	 b r e a k ; 
 	 	 } 
 	 } 
 
 	 v o i d   _ u p d a t e _ r e n d e r ( ) 
 	 { 
 	 	 f o r   ( a u t o &   w i n   :   m _ w i n d o w M a p ) 
 	 	 { 
 	 	 	 w i n - > R e n d e r ( ) ; 
 	 	 } 
 	 	 Q A p p l i c a t i o n : : p r o c e s s E v e n t s ( ) ; 
 	 } 
 
 
 	 v t k S m a r t P o i n t e r < v t k P o l y D a t a >   _ g e t _ d a t a ( c o n s t   Q S t r i n g &   d a t a T y p e ) 
 	 { 
 	 	 r e t u r n   m _ d a t a M a p . v a l u e ( d a t a T y p e ) ; 
 	 } 
 	 v t k S m a r t P o i n t e r < v t k M a p p e r >   _ g e t _ m a p p e r ( c o n s t   Q S t r i n g &   d a t a T y p e ,   c o n s t   Q S t r i n g &   v i e w T y p e ) 
 	 { 
 	 	 r e t u r n   m _ m a p p e r M a p . v a l u e ( v i e w T y p e ) . m _ m a p p e r . v a l u e ( d a t a T y p e ) ; 
 	 } 
 	 v t k S m a r t P o i n t e r < v t k A c t o r >   _ g e t _ a c t o r ( c o n s t   Q S t r i n g &   d a t a T y p e ,   c o n s t   Q S t r i n g &   v i e w T y p e ) 
 	 { 
 	 	 r e t u r n   m _ a c t o r M a p . v a l u e ( v i e w T y p e ) . m _ a c t o r . v a l u e ( d a t a T y p e ) ; 
 	 } 
 	 v t k S m a r t P o i n t e r < v t k R e n d e r e r >   _ g e t _ r e n d e r ( c o n s t   Q S t r i n g &   v i e w T y p e ) 
 	 { 
 	 	 r e t u r n   m _ r e n d e r M a p . v a l u e ( v i e w T y p e ) ; 
 	 } 
 	 v t k S m a r t P o i n t e r < v t k G e n e r i c O p e n G L R e n d e r W i n d o w >   _ g e t _ w i n d o w ( c o n s t   Q S t r i n g &   v i e w T y p e )   { 
 	 	 r e t u r n   m _ w i n d o w M a p . v a l u e ( v i e w T y p e ) ; 
 	 } 
 } ; 
 
 C B C T M o d e l C o n t r o l l e r : : C B C T M o d e l C o n t r o l l e r ( U i : : M a i n W i n d o w *   p a r e n t U I ) 
 	 :   P D a t a ( n e w   I n t e r n a l ( t h i s ) ) 
 { 
 	 P D a t a - > m _ p a r e n t U I   =   p a r e n t U I ; 
 } 
 
 C B C T M o d e l C o n t r o l l e r : : ~ C B C T M o d e l C o n t r o l l e r ( ) 
 { 
 } 
 
 b o o l   C B C T M o d e l C o n t r o l l e r : : i n i t i a l i z e ( ) 
 { 
 	 r e t u r n   P D a t a - > _ i n i t i a l i z e ( ) ; 
 } 
 v o i d   C B C T M o d e l C o n t r o l l e r : : o n _ R o t a t e _ P a n o O b j e c t ( c o n s t   i n t &   v a l ) 
 { 
 	 a u t o   a n g l e   =   c e i l ( v a l   /   5 ) ; 
 	 q D e b u g ( )   < <   " i m a g e   C o u n t   :   "   < <   v a l   < <   " a n g l e   :   "   < <   a n g l e ; 
 
 	 i f   ( v a l   = =   1 7 5 0 ) 
 	 	 a n g l e   =   3 6 0 ; 
 
 	 i f   ( P D a t a - > i s R u n n i n g _ P a n o ) 
 	 	 r e t u r n ; 
 	 P D a t a - > i s R u n n i n g _ P a n o   =   t r u e ; 
 
 	 P D a t a - > _ R o t a t e _ P a n o ( a n g l e ) ; 
 
 	 P D a t a - > i s R u n n i n g _ P a n o   =   f a l s e ; 
 
 } 
 v o i d   C B C T M o d e l C o n t r o l l e r : : o n _ T r a n s l a t e _ C e p h O b j e c t ( c o n s t   i n t &   v a l ) 
 { 
 
 	 i n t   c o u n t   =   0 . 8 4 8   *   v a l ; 
 
 	 q D e b u g ( )   < <   " i m a g e   :   "   < <   v a l   < <   " c o u n t   :   "   < <   c o u n t ; 
 	 / /   Z ��  + ٳ��  
 	 i f   ( 0   <   c o u n t   & &   c o u n t   < =   2 6 0 ) 
 	 { 	 
 	 	 i f   ( P D a t a - > i s R u n n i n g _ C e p h ) 
 	 	 	 r e t u r n ; 
 	 	 P D a t a - > i s R u n n i n g _ C e p h   =   t r u e ; 
 
 	 	 P D a t a - > _ T r a n s l a t e _ C e p h ( c o u n t ) ; 
 
 	 	 P D a t a - > i s R u n n i n g _ C e p h   =   f a l s e ; 
 	 } 
 	 e l s e   i f   ( 2 6 0   <   c o u n t   & &   c o u n t   < =   5 2 0 ) 
 	 { 
 	 	 i f   ( P D a t a - > i s R u n n i n g _ C e p h ) 
 	 	 	 r e t u r n ; 
 	 	 P D a t a - > i s R u n n i n g _ C e p h   =   t r u e ; 
 
 	 	 P D a t a - > _ T r a n s l a t e _ C e p h ( 5 2 0   -   c o u n t ) ; 
 	 	 P D a t a - > i s R u n n i n g _ C e p h   =   f a l s e ; 
 	 } 
 	 e l s e   i f   ( 5 2 0   <   c o u n t   & &   c o u n t   < =   7 9 0 ) 
 	 { 
 
 	 	 i f   ( P D a t a - > i s R u n n i n g _ C e p h ) 
 	 	 	 r e t u r n ; 
 	 	 P D a t a - > i s R u n n i n g _ C e p h   =   t r u e ; 
 
 	 	 P D a t a - > _ T r a n s l a t e _ C e p h ( - ( c o u n t   -   5 2 0 ) ) ; 
 	 	 P D a t a - > i s R u n n i n g _ C e p h   =   f a l s e ; 
 	 } 
 	 e l s e   i f   ( 7 9 0   <   c o u n t   & &   c o u n t   < =   1 0 6 0 ) 
 	 { 
 
 	 	 i f   ( P D a t a - > i s R u n n i n g _ C e p h ) 
 	 	 	 r e t u r n ; 
 	 	 P D a t a - > i s R u n n i n g _ C e p h   =   t r u e ; 
 
 	 	 P D a t a - > _ T r a n s l a t e _ C e p h ( - ( 1 0 6 0   -   c o u n t ) ) ; 
 	 	 P D a t a - > i s R u n n i n g _ C e p h   =   f a l s e ; 
 	 } 
 
 } 
 v o i d   C B C T M o d e l C o n t r o l l e r : : o n _ M a i n P u s h B u t t o n _ c l i c k e d ( ) 
 { 
 	 q D e b u g ( )   < <   " M a i n   P u s h   B t n ! ! " ; 
 	 P D a t a - > _ o n _ M a i n P u s h B u t t o n _ c l i c k e d ( ) ; 
 } 
 
 v o i d   C B C T M o d e l C o n t r o l l e r : : o n _ S u b P u s h B u t t o n _ c l i c k e d ( ) 
 { 
 	 q D e b u g ( )   < <   " S u b   P u s h   B t n ! ! " ; 
 	 P D a t a - > _ o n _ S u b P u s h B u t t o n _ c l i c k e d ( ) ; 
 } 
 
 v o i d   C B C T M o d e l C o n t r o l l e r : : o n _ A s c e n d i n g P u s h B u t t o n _ p r e s s e d ( ) 
 { 
 	 P D a t a - > _ o n _ A s c e n d i n g P u s h B u t t o n _ p r e s s e d ( ) ; 
 } 
 
 v o i d   C B C T M o d e l C o n t r o l l e r : : o n _ D e s c e n d i n g P u s h B u t t o n _ p r e s s e d ( ) 
 { 
 	 P D a t a - > _ o n _ D e s c e n d i n g P u s h B u t t o n _ p r e s s e d ( ) ; 
 } 
 
 v o i d   C B C T M o d e l C o n t r o l l e r : : o n _ C a p t u r e R e s e t P u s h B u t t o n _ V T K _ c l i c k e d ( ) 
 { 
 	 P D a t a - > _ o n _ C a p t u r e R e s e t P u s h B u t t o n _ V T K _ c l i c k e d ( ) ; 
 } 
 
 v o i d   C B C T M o d e l C o n t r o l l e r : : o n _ C a p t u r e R e a d y P u s h B u t t o n _ V T K _ c l i c k e d ( ) 
 { 
 	 P D a t a - > _ o n _ C a p t u r e R e a d y P u s h B u t t o n _ V T K _ c l i c k e d ( ) ; 
 } 
 