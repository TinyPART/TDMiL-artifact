#include <stddef.h>
#include <stdint.h>
#include <dlpack/dlpack.h>
const size_t fc3_weight_len = 840;
float fc3_weight[] ={0.12041052430868149, -0.061160728335380554, 0.10220056027173996, -0.04574887827038765, 0.08844321221113205, 0.06296604871749878, 0.021865230053663254, -0.05379167199134827, -0.05969185754656792, 0.16529227793216705, 0.050654128193855286, -0.02973487786948681, 0.14301873743534088, 0.05398143082857132, -0.028026392683386803, -0.06244373694062233, 0.13504105806350708, -0.047822289168834686, -0.011889435350894928, -0.02536875382065773, -0.02593202330172062, 0.09979799389839172, -0.135823056101799, 0.06930815428495407, 0.03426073119044304, -0.10651234537363052, 0.10023409128189087, 0.07664747536182404, -0.013164281845092773, -0.006739519536495209, 0.0325716957449913, -0.05641636624932289, 0.23582541942596436, -0.0783233642578125, -0.06543111056089401, 0.11100579053163528, 0.09046091884374619, -0.01248828787356615, 0.24755574762821198, -0.009867524728178978, 0.02371451072394848, -0.09125559777021408, -0.04552290961146355, -0.06922236829996109, 0.00622034864500165, 0.06251564621925354, -0.05525994300842285, -0.04158812761306763, -0.0339997261762619, -0.07376164942979813, -0.054080862551927567, 0.1877056211233139, 0.16742372512817383, -0.01972566731274128, -0.0802309587597847, -0.08474572002887726, 0.04402640834450722, -0.02564876340329647, 0.11689502745866776, 0.0042580836452543736, 0.06567651033401489, 0.01724933832883835, 0.059276677668094635, 0.33769896626472473, 0.07248462736606598, -0.04692385718226433, 0.018281804397702217, -0.025101734325289726, -0.028391407802700996, -0.05272354185581207, 0.05512609705328941, 0.01059579849243164, 0.046083640307188034, -0.02371416985988617, 0.028739741072058678, 0.08422538638114929, 0.05063747242093086, 0.02495257928967476, 0.08268163353204727, 0.06218750774860382, -0.0037566807586699724, -0.01989787444472313, 0.03710018843412399, -0.11286883056163788, -0.22033794224262238, 0.09254832565784454, -0.17666687071323395, -0.06805416196584702, -0.07012297958135605, -0.10378061234951019, 0.07619141787290573, -0.030336204916238785, 0.3400038778781891, -0.026194768026471138, -0.14996781945228577, 0.021992258727550507, -0.125462144613266, 0.05124269425868988, 0.027394942939281464, -0.04032411426305771, -0.056697554886341095, -0.017952410504221916, 0.0022544346284121275, 0.07395313680171967, -0.06364675611257553, 0.027170833200216293, 0.28832435607910156, 0.022573035210371017, -0.0075484225526452065, -0.23099535703659058, -0.16575343906879425, 0.17735643684864044, -0.06338321417570114, -0.25812938809394836, -0.09201429039239883, -0.005755359306931496, 0.007940400391817093, -0.0372081957757473, -0.011567721143364906, 0.2350034862756729, 0.25686344504356384, 0.3015851080417633, 0.01900460757315159, -0.11112915724515915, 0.3370033800601959, -0.02683502808213234, 0.14738570153713226, 0.10717608779668808, 0.1264537125825882, 0.11890126764774323, -0.007133450359106064, 0.18083401024341583, -0.10895486921072006, -0.050255026668310165, -0.005007245112210512, -0.17395785450935364, -0.29764318466186523, -0.2470136433839798, -0.07052885740995407, 0.3705653250217438, -0.2240292876958847, 0.2414635866880417, -0.046395234763622284, -0.01835719309747219, -0.04192701727151871, 0.1064218133687973, 0.14339980483055115, -0.006287213880568743, 0.04848192632198334, 0.2361517995595932, 0.14485475420951843, -0.08222981542348862, 0.3712766170501709, -0.10489576309919357, -0.02589019201695919, -0.20293498039245605, -0.21316084265708923, -0.05851341411471367, -0.20303058624267578, -0.03327205032110214, 0.013420958071947098, 0.16578276455402374, -0.028463253751397133, -0.1152789443731308, -0.03705700859427452, 0.010085317306220531, -0.007978493347764015, 0.1022193506360054, 0.28283101320266724, 0.16392071545124054, -0.11628194153308868, 0.24068787693977356, -0.01786118559539318, -0.1092698872089386, 0.06071416288614273, 0.08739139884710312, -0.07721030712127686, -0.09857695549726486, 0.046011783182621, 0.1046094298362732, 0.12145920842885971, 0.10135799646377563, -0.016314087435603142, 0.13154549896717072, 0.012908892706036568, 0.07003715634346008, -0.009140868671238422, -0.13444185256958008, 0.2565872371196747, 0.05849079042673111, 0.027791496366262436, 0.10050920397043228, 0.09538204222917557, -0.18241874873638153, 0.005782375577837229, -0.08755890280008316, -0.20957836508750916, -0.01222534105181694, -0.1972706913948059, 0.07206548005342484, 0.23599742352962494, 0.03123585321009159, 0.07682640850543976, 0.07219064980745316, -0.2560127079486847, -0.03770554065704346, 0.08124951273202896, -0.10272885859012604, 0.24547179043293, -0.007638230454176664, -0.1863725632429123, 0.15139032900333405, 0.12601631879806519, 0.23576100170612335, 0.0201868899166584, -0.36284658312797546, 0.0676698163151741, 0.052064597606658936, 0.03168503940105438, -0.1964702308177948, 0.028950313106179237, -0.09975054115056992, -0.05124096944928169, 0.4644205570220947, -0.3348958492279053, -0.25063079595565796, -0.18271858990192413, 0.0980355516076088, 0.07392003387212753, 1.4842040400253609e-05, 0.001769844675436616, -0.024445611983537674, 0.0807817205786705, -0.16209658980369568, -0.33705002069473267, 0.28432485461235046, -0.05134817212820053, -0.012507935054600239, 0.1208290308713913, -0.15131781995296478, 0.2433200627565384, 0.14061996340751648, 0.1582566350698471, 0.10977643728256226, -0.18139930069446564, 0.033263273537158966, -0.054701194167137146, -0.28540873527526855, -0.0750390961766243, 0.1911763846874237, 0.08181566745042801, -0.08260463923215866, -0.08427754789590836, 0.1483505517244339, -0.16935589909553528, 0.07436361908912659, -0.03426321968436241, -0.19097338616847992, 0.07080172747373581, 0.07288123667240143, -0.2365572601556778, -0.1030430793762207, -0.02005867473781109, -0.08388984203338623, 0.15078669786453247, -0.08850586414337158, -0.07090907543897629, -0.13471809029579163, -0.02457951381802559, 0.07746260613203049, 0.05844052508473396, -0.12129795551300049, 0.20266413688659668, 0.013644810765981674, 0.22968775033950806, 0.12123885750770569, -0.046002618968486786, 0.32105252146720886, -0.018150489777326584, 0.05813206732273102, -0.15629792213439941, 0.043148159980773926, -0.17388290166854858, 0.015555528923869133, -0.22006404399871826, -0.1571003496646881, 0.3479151725769043, -0.1657589077949524, 0.22904421389102936, 0.28457731008529663, -0.15067818760871887, 0.1207994595170021, -0.2789933383464813, -0.03478935733437538, 0.21536441147327423, -0.2520189881324768, 0.07710430026054382, 0.0315057709813118, 0.09265463799238205, 0.026648061349987984, -0.046368230134248734, 0.19866783916950226, -0.2619470953941345, 0.16788126528263092, 0.17127279937267303, -0.0032241090666502714, 0.2132084220647812, -0.09595399349927902, -0.1615300178527832, 0.07815471291542053, -0.19801823794841766, 0.08379210531711578, 0.04423525184392929, -0.042667362838983536, -0.10382317006587982, -0.2697628140449524, 0.06780297309160233, 0.13780532777309418, -0.22426290810108185, -0.022065123543143272, 0.03069497086107731, 0.009651361033320427, -0.0014733506832271814, 0.17888259887695312, 0.08891680091619492, 0.2236495167016983, -0.11478865891695023, -0.025677287951111794, 0.0856776237487793, -0.11161266267299652, 0.07290103286504745, 0.2674850821495056, 0.02228500135242939, 0.10394557565450668, -0.059167709201574326, -0.18301509320735931, 0.04387867823243141, -0.16659915447235107, -0.17170579731464386, 0.003619943978264928, 0.002766396151855588, 0.28445005416870117, 0.036553945392370224, 0.20200498402118683, 0.0066827512346208096, 0.034608159214258194, 0.21391455829143524, -0.0052195461466908455, -0.03343873471021652, -0.00520849647000432, -0.09542298316955566, -0.06441160291433334, 0.01063314825296402, 0.04858795925974846, -0.13023178279399872, 0.14183947443962097, -0.08383014798164368, -0.09727048128843307, 0.10407163202762604, 0.0031838815193623304, -0.04819213226437569, 0.13788186013698578, -0.04386129602789879, -0.09044915437698364, 0.29940804839134216, 0.24032089114189148, -0.2679053246974945, -0.0773824006319046, -0.14396744966506958, -0.002393946051597595, 0.07328356057405472, 0.09369049966335297, -0.06696086376905441, -0.08746389299631119, 0.05455393344163895, 0.014654802158474922, -0.0584230050444603, 0.053154356777668, -0.2720705568790436, 0.3069022297859192, 0.0287335105240345, 0.3984682857990265, -0.0368892140686512, -0.06523523479700089, 0.09284111112356186, -0.12021851539611816, 0.08908837288618088, 0.03514524921774864, -0.3393544554710388, 0.22822889685630798, 0.12945260107517242, 0.04240885749459267, 0.09376516193151474, -0.14351803064346313, -0.19946277141571045, -0.040325675159692764, -0.012980489060282707, -0.028534535318613052, -0.07778062671422958, -0.20474021136760712, 0.10508536547422409, 0.10340231657028198, 0.22088560461997986, -0.07344833016395569, 0.15607990324497223, 0.07211563736200333, -0.24878905713558197, -0.36410197615623474, -0.22992561757564545, 0.11743659526109695, 0.0519564151763916, 0.02937859669327736, 0.07270690053701401, 0.346436470746994, -0.04784228280186653, -0.11224958300590515, -0.06214767321944237, -0.07173838466405869, 0.09427488595247269, 0.01722848415374756, -0.001441689906641841, 0.0036938702687621117, -0.045452382415533066, 0.14973780512809753, -0.0788261741399765, 0.03680070489645004, 0.21826811134815216, 0.027412451803684235, 0.13548718392848969, -0.1345377266407013, -0.1915842592716217, -0.01783854328095913, 0.06097472459077835, -0.12735624611377716, -0.015222716145217419, 0.023243257775902748, -0.16062608361244202, 0.08910329639911652, -0.15393583476543427, -0.07830134779214859, 0.019632583484053612, 0.029932178556919098, -0.2999480664730072, -0.05565783008933067, 0.05670618638396263, 0.0527808703482151, -0.27024656534194946, -0.10678647458553314, 0.2729837894439697, 0.04257892072200775, 0.06783998012542725, -0.019283993169665337, -0.1082657128572464, 0.21019743382930756, 0.21685628592967987, 0.0067507014609873295, -0.10788125544786453, 0.03866393864154816, -0.1388925462961197, 0.0893983319401741, 0.0767350122332573, -0.07592742145061493, -0.04983537271618843, -0.019764767959713936, 0.22378964722156525, -0.3294672667980194, 0.010825211182236671, 0.3777850568294525, -0.051499102264642715, 0.18736110627651215, -0.14626380801200867, -0.09861979633569717, 0.2886856198310852, 0.3065345287322998, 0.25680065155029297, -0.013667832128703594, -0.015262232162058353, 0.47181037068367004, 0.1954432874917984, 0.04601667448878288, 0.08578085154294968, -0.06143777072429657, -0.028681209310889244, -0.11793745309114456, 0.02490496076643467, -0.010643335059285164, -0.06458453088998795, 0.06595001369714737, -0.21431487798690796, 0.03393763303756714, -0.024245401844382286, 0.17747820913791656, -0.26091426610946655, 0.005511217284947634, 0.06491724401712418, 0.02910228818655014, -0.12579397857189178, -0.1220972090959549, -0.07957421988248825, 0.06936506181955338, -0.053777724504470825, -0.05594944953918457, 0.008911493234336376, 0.08383314311504364, -0.20690931379795074, -0.3527926206588745, 0.022844048216938972, 0.18936127424240112, 0.04462628439068794, -0.06295008212327957, -0.04471990466117859, 0.012540992349386215, 0.03557097539305687, 0.14934206008911133, 0.2867013216018677, -0.01788541115820408, -0.04885648190975189, -0.11400402337312698, 0.029192809015512466, 0.21655221283435822, 0.0422024242579937, -0.08275015652179718, -0.03134474903345108, -0.11810068041086197, -0.34257981181144714, 0.05157618224620819, -0.024414295330643654, -0.11778111010789871, 0.03451434150338173, -0.28596144914627075, 0.1870589703321457, -0.07951261103153229, 0.10485958307981491, -0.26940345764160156, -0.22489450871944427, -0.04650340601801872, -0.2146168351173401, 0.17935995757579803, -0.1502261757850647, 0.00023055248311720788, -0.01274841371923685, -0.07675641775131226, -0.03303904831409454, 0.04549515247344971, 0.02391088381409645, -0.09076736122369766, -0.1461695432662964, 0.08539321273565292, -0.11129827052354813, -0.23150749504566193, -0.013939018361270428, 0.24147187173366547, 0.04204408824443817, 0.16827110946178436, -0.026803892105817795, 0.0023774041328579187, 0.1172717958688736, 0.23912285268306732, 0.042899589985609055, -0.17934638261795044, -0.23965707421302795, 0.11672932654619217, 0.204157292842865, -0.010238579474389553, -0.03126230835914612, 0.04631344974040985, -0.13697265088558197, -0.10937082022428513, -0.09386593848466873, 0.2536356449127197, 0.015483240596950054, -0.1287422627210617, -0.05847916379570961, -0.09486842155456543, 0.3885296881198883, -0.1940632462501526, 0.10521812736988068, -0.20332132279872894, 0.2619342803955078, 0.02555857039988041, -0.00906834565103054, 0.2387489527463913, -0.01764926128089428, 0.10435397177934647, -0.10893411189317703, 0.04961211234331131, 0.03136064484715462, 0.15931421518325806, -0.1135140135884285, 0.29514074325561523, -0.0012228375999256968, -0.10189991444349289, -0.10547672212123871, 0.18049617111682892, 0.11751818656921387, -0.05444417521357536, -0.012639480642974377, 0.027341259643435478, 0.11029262840747833, -0.02656467817723751, -0.16251720488071442, -0.06232686713337898, 0.0811057761311531, 0.08023199439048767, -0.08144617080688477, 0.07316811382770538, 0.175625741481781, 0.15251480042934418, 0.08327406644821167, -0.05922922119498253, -0.009528327733278275, 0.0817764475941658, -0.0016274383524432778, 0.3476181626319885, -0.05082748457789421, 0.19092784821987152, 0.053088851273059845, -0.2492971122264862, 0.18865062296390533, 0.04109516367316246, -0.03111433982849121, 0.09977404773235321, 0.11394541710615158, 0.07381456345319748, 0.10104978084564209, 0.285974383354187, 0.021908964961767197, 0.0565371960401535, 0.037228651344776154, 0.09926718473434448, 0.09179414808750153, -0.12760542333126068, -0.17388661205768585, -0.15721189975738525, 0.02646535262465477, -0.04046108201146126, 0.07134956866502762, -0.06780160218477249, 0.3784579038619995, -0.16912025213241577, -0.18779879808425903, -0.03890689089894295, -0.23173099756240845, 0.2943386733531952, -0.024144256487488747, -0.13741546869277954, -0.1695559322834015, -0.05624875798821449, -0.017924567684531212, -0.05210627242922783, -0.034529369324445724, 0.16836898028850555, 0.08477775007486343, -0.14685116708278656, 0.10487156361341476, 0.1298704743385315, 0.19756241142749786, -0.05430159717798233, -0.03144887834787369, -0.025194963440299034, -0.20688721537590027, 0.39116281270980835, -0.04464776813983917, 0.06911908835172653, -0.1273079365491867, -0.08333708345890045, -0.0035308049991726875, -0.14328612387180328, -0.06769182533025742, -0.13614387810230255, 0.06600402295589447, 0.12405332177877426, 0.08763904869556427, 0.5867131948471069, 0.012508908286690712, -0.01992502436041832, -0.07556294649839401, -0.26754045486450195, 0.10825514048337936, 0.05249059945344925, 0.16158713400363922, -0.14856727421283722, -0.08445798605680466, -0.08660682290792465, -0.021970577538013458, 0.04646958038210869, 0.05253342539072037, 0.24913206696510315, 0.016791829839348793, 0.0519421361386776, -0.04936998710036278, 0.01353867631405592, -0.025312740355730057, 0.01617039367556572, -0.09221123903989792, -0.04984284192323685, 0.07658644020557404, -0.1194877102971077, -0.028295891359448433, -0.03305641934275627, -0.11366954445838928, 0.2155533730983734, -0.19379888474941254, -0.06768645346164703, -0.11757022142410278, -0.060237959027290344, -0.20206883549690247, -0.20046067237854004, 0.3373645842075348, -0.24789150059223175, -0.24132183194160461, 0.04315624386072159, -0.08628661185503006, 0.15544380247592926, -0.25647303462028503, 0.14446833729743958, -0.08981569111347198, 0.19003546237945557, -0.07275734096765518, 0.2300097942352295, 0.07619849592447281, 0.26782962679862976, -0.11659685522317886, 0.19114214181900024, 0.17906419932842255, 0.18789443373680115, -0.08940032869577408, -0.16889680922031403, 0.2577798664569855, -0.12303052842617035, -0.2970350384712219, 0.08210152387619019, -0.1311534345149994, -0.004373990930616856, -0.1685422658920288, -0.153644859790802, 0.055822521448135376, -0.0711485967040062, -0.26811790466308594, 0.008463279344141483, -0.05324247106909752, -0.053542982786893845, 0.1399780809879303, 0.13384684920310974, 0.11071354895830154, 0.11918377131223679, -0.053354546427726746, 0.07905355840921402, 0.01737295463681221, 0.1756763607263565, 0.016170145943760872, 0.13644196093082428, 0.044957831501960754, -0.23134852945804596, -0.08910256624221802, 0.11644738167524338, 0.09208890050649643, 0.018053974956274033, -0.20986835658550262, -0.15768371522426605, -0.08289479464292526, 0.4443379044532776, -0.07184405624866486, -0.14330703020095825, 0.11739382892847061, -0.07674849778413773, -0.29875099658966064, 0.018110590055584908, 0.046997684985399246, -0.004590285010635853, -0.06395823508501053, -0.05161729082465172, -0.0006325970171019435, -0.07656832784414291, -0.1877581626176834, 0.06571453809738159, 0.19787338376045227, -0.16270263493061066, 0.03649543225765228, -0.222936749458313, 0.045818835496902466, 0.27736344933509827, 0.25956496596336365, 0.019387109205126762, 0.33544930815696716, 0.497858464717865, -0.12409713864326477, 0.09404076635837555, 0.0769796371459961, 0.12833714485168457, -0.11769255250692368, -0.02323516458272934, 0.05363643541932106, -0.23476272821426392, -0.05321355164051056, 0.04979415610432625, 0.012124395929276943, -0.0741838663816452, 0.04113766551017761, -0.34046298265457153, 0.1253504604101181, 0.24525782465934753, 0.05065663158893585, 0.11191460490226746, -0.05907715484499931, -0.2628141939640045, 0.0030526937916874886, -0.061779484152793884, -0.08693483471870422, -0.16587868332862854, 0.17536868155002594, -0.13546055555343628, 0.03005877509713173, 0.09691318869590759, 0.13684841990470886, -0.11979250609874725, -0.03413134068250656, -0.06547117978334427, 0.10062125325202942, -0.28272560238838196, -0.0010621288092806935, -0.2172938883304596, 0.06045560538768768, -0.0019120757933706045, 0.1698514074087143, -0.07321687787771225, -0.16417063772678375, 0.07755208015441895, -0.28775838017463684, 0.09906497597694397, -0.10497479885816574, -0.0907997414469719, 0.17744362354278564, 0.2511161267757416, 0.05178908631205559, 0.37787318229675293, -0.007471085991710424, 0.12151476740837097, 0.053938064724206924, 0.3021511137485504, };

