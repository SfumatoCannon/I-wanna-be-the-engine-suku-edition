#include "RoomTitle.h"
#include "RoomSelectSave.h"

RoomTitle::RoomTitle()
{
	MapLoader::loadFromJtool(this, "-e91oe1ib-eb1oe1h21d8-ei1od-et1oa-fb1o4-fs1nq1gv-g81ne1mp-gb1n3-g01mh1gv1401s0-fn1md-fc1ma1h0-eu1m6-eg1m6-e51m61he1e4-dv1m8-ds1m81ea-hl1jp-hk1jp-hg1jm-h91jj-gu1je-gc1j6-fp1it-f91im-eq1ig-ef1ic-e81ib-ci1jn-cj1jl1ck1aq-cm1jf-cr1j7-d21it-d91ih-dg1i41ak-do1hq-e01hj1401s0-fv1gv-fm1h0-f11h0-em1h119d-e11h3-dk1h31ck-d51h3-cl1h4-c41h61f1-bl1h8-b61hb-an1he-a81hh-9p1hk-9b1hn-8t1hq-8h1hs-881hu-831hu-821hu-c11f219v-cb1ev-co1es-d61en-dh1eh-ec1dt-ee1dp1dk-ed1de19p-e71d3-e31cr1a5-dt1cl-d81ck-cs1ck-ce1ck-ca1ck-f717a17c17h17r-f6188-f218m-es192-dq1ad-d71ao-cu1aq-cn1ar-cf1an-c91ad-bo19f-bg190-b518h-ao185-ab17s-a017o1401s0-9n17m-9e17l-9117l-8n17o-8e17s-86180-801871401s0-7q18g-7m18r1a3-7j196-7i19e19n-7r1ad-7u1ai1aj-m01r01q01p01o01n01m01l01k01j01i01h01g01f01e01d01c01b01a01901801701601501401s0-l01401s0-k01401s0-j01401s0-i01401s0-h01401s0-f01401s0-d01401s0-c01401s0-b01401s0-901401s0-701401s0-601401s0-501401s0-401s01r01q01p01o01n01m01l01k01j01i01h01g01f01e01d01c01b01a0190180170160150140");
}

void RoomTitle::onUpdateStart()
{
	if (input::isKeyDown(VK_SHIFT))
		gotoRoom<RoomSelectSave>();
}
