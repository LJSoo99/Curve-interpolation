//
//  main.cpp
//  CurveInter
//
//  Created by Hyun Joon Shin on 2024/03/15.
//

#define JGL2_IMPLEMENTATION
#include <JGL2/JGL.hpp>
//#include <Eigen/dense>

using jm::vec2;
using namespace JGL2;

enum {
	LINEAR,
	LAGLANGIAN,
	BEZIER,
	CATMULL,
	NATURAL,
	BSPLINE,
};

enum {
	DRAW_LINES,
	DRAW_DOTS,
};

int curveType = LINEAR;
int drawType = DRAW_LINES;
bool closed = false;

std::vector<vec2> srcPts;
std::vector<vec2> samplePts;

template<typename T> T Bezier(const T& p0, const T& p1, const T& p2, const T& p3, float t) {
	float t1 = 1 - t;
	return t1 * t1 * t1 * p0 + 3 * t1 * t1 * t * p1 + 3 * t1 * t * t * p2 + t * t * t * p3;
}


std::vector<vec2> evaluateCurve( int curveType, const std::vector<vec2>& srcPts, bool closed, const std::vector<float>& samples ) {
	std::vector<vec2> ret;
	switch( curveType ) {
		case LAGLANGIAN: {
			for(auto T : samples) {
				vec2 pt(0);
				for( int i=0; i<srcPts.size(); i++ ) {
					float w=1;
					for( int j=0; j<srcPts.size(); j++ )
						if( i!=j ) w*=(T-j)/(i-j);
					pt+= w * srcPts[i];
				}
				ret.push_back(pt);
			}
		} break;
		case BEZIER: {
			for( float t = 0; t<=1.0; t+=0.02f) {
				vec2 pt = (1-t)*(1-t)*(1-t)*srcPts[0] + 3*(1-t)*(1-t)*t*srcPts[1]
					+ 3*t*t*(1-t)*srcPts[2] + t*t*t*srcPts[3];
				ret.push_back(pt);
			}
		} break;
		case CATMULL: {
			for(auto T : samples) {
				int k = int(floor(T));
				float t = T-k;

				vec2 v0, v1;
				if( k>0 ) v0 = ( srcPts[k+1] - srcPts[k-1] )/2;
				else {
					//p(t) = a*t*t + b*t + c;    

					//p(0) = c = srcPts[k];
					//p(1) = a+b = srcPts[k+1] - srcPts[k];
					//p(2) //= 4*a+2*b  = srcPts[k+2] - srcPts[k];
					//p(1) //= 2*a+2*b = 2*srcPts[k+1] - 2*srcPts[k];
					//vec2 a =  (srcPts[k+2] -2*srcPts[k+1] + srcPts[k])/2;
					v0 = (4*srcPts[k+1] - 3*srcPts[k]-srcPts[k+2])/2;
				}
				if( k+2<srcPts.size() ) v1 = ( srcPts[k+2] - srcPts[k] )/2;
				else v1 = -(4*srcPts[k] - 3*srcPts[k+1]-srcPts[k-1])/2;

				vec2 pt = Bezier(srcPts[k], srcPts[k]+v0/3, srcPts[k+1]-v1/3, srcPts[k+1],t);

				//vec2 pt = (1-t)*srcPts[k] + t*srcPts[k+1];

				ret.push_back(pt);
			} 
		} break;
		case NATURAL: {
			int n=srcPts.size()-1;
			std::vector<vec2> D(n+1,0);
			for( int iter=0; iter<10; iter++ ) {
				D[0] = ( 3*( srcPts[1] - srcPts[0]) - D[1] )/2;
				for( int i=1; i<n; i++ )
					D[i] = ( 3*(srcPts[i+1]-srcPts[i-1]) -D[i+1]-D[i-1] )/4;
				D[n] = ( 3*( srcPts[n] - srcPts[n-1]) -D[n-1])/2;
			}
			for(auto T : samples) {
				int k = int(floor(T));
				float t = T-k;
				vec2 a, b, c, d;
				a = srcPts[k];
				b = D[k];
				c = 3*(srcPts[k+1]-srcPts[k])-2*D[k]-D[k+1];
				d =-2*(srcPts[k+1]-srcPts[k])+D[k]+D[k+1];
				vec2 pt = a+b*t+c*t*t+d*t*t*t;

				ret.push_back(pt);
			}

		} break;
		case BSPLINE: {
			for (size_t i = 0; i + 3 < srcPts.size(); i++) {
				for (float t = 0; t < 1.0; t += 0.01f) {
					float b0 = (1 - t) * (1 - t) * (1 - t) / 6.0f;
					float b1 = (3 * t * t * t - 6 * t * t + 4) / 6.0f;
					float b2 = (-3 * t * t * t + 3 * t * t + 3 * t + 1) / 6.0f;
					float b3 = t * t * t / 6.0f;

					vec2 pt = b0 * srcPts[i] + b1 * srcPts[i + 1] + b2 * srcPts[i + 2] + b3 * srcPts[i + 3];
					ret.push_back(pt);
				}
			}
		} break;
		case LINEAR :
		default: {
			for(auto T : samples) {
				int k = int(floor(T));
				float t = T-k;

				vec2 pt = (1-t)*srcPts[k] + t*srcPts[k+1];

				ret.push_back(pt);
			}
		}
	}
	return ret;
}
void updateCurve( int curveType, const std::vector<vec2>& srcPts, bool closed ) {
	samplePts.clear();
	std::vector<float> sampleT;
	if( !closed ) {
		for(float t=0; t<=srcPts.size()-1; t+=0.1f )
			sampleT.push_back(t);
	}
	else {
		for(float t=0; t<=srcPts.size(); t+=0.1f )
			sampleT.push_back(t);
	}
	samplePts = evaluateCurve(curveType, srcPts, closed, sampleT);
}

struct CurveWidget : Widget {
	CurveWidget(float x, float y, float w, float h, const std::string& title = "" )
	: JGL2::Widget(x,y,w,h,title){}
	virtual void		drawBox(NVGcontext* vg, const JGL2::rct_t& r) override {
		nvgSave(vg);
		nvgBeginPath( vg );
		nvgRect( vg, r.x, r.y, r.w, r.h );
		nvgFillColor( vg, nvgRGBAf(0,0,0,1));
		nvgFill( vg );
		nvgRestore(vg);
	}
	
	virtual void drawContents(NVGcontext* vg, const JGL2::rct_t& r, JGL2::align_t align ) override {
		nvgSave(vg);
		if( drawType == DRAW_LINES ) {
			nvgBeginPath( vg );
			nvgMoveTo( vg, samplePts[0].x, samplePts[0].y );
			for( auto i=1; i<samplePts.size(); i++ ) {
				nvgLineTo( vg, samplePts[i].x, samplePts[i].y );
			}
			nvgStrokeColor(vg, nvgRGBAf(0,.8f,1,1));
			nvgStrokeWidth(vg, 2);
			nvgStroke( vg );
		}
		else {
			nvgFillColor(vg, nvgRGBAf(0,1,.3f,1));
			nvgBeginPath( vg );
			for( auto i=0; i<samplePts.size(); i++ ) {
				nvgCircle(vg, samplePts[i].x, samplePts[i].y, 1);
			}
			nvgFill( vg );
		}

		for( auto i=0; i<srcPts.size(); i++ )
			if( i!= underPt ) {
				nvgBeginPath( vg );
				nvgCircle( vg, srcPts[i].x, srcPts[i].y, 5 );
				nvgFillColor( vg, nvgRGBAf(1,1,0,.8f));
				nvgFill( vg );
			}
		if( underPt>=0 ) {
			nvgBeginPath( vg );
			nvgCircle( vg, srcPts[underPt].x, srcPts[underPt].y, 5 );
			nvgFillColor( vg, nvgRGBAf(1,.1f,0,.8f));
			nvgFill( vg );
		}
		nvgRestore(vg);
	}
	virtual bool handle( JGL2::event_t event ) override {
		vec2 pt = JGL2::_JGL::eventPt();
		switch( event ) {
			case JGL2::event_t::MOVE : {
				int oldPt = underPt;
				underPt = -1;
				for( auto i=0; i<srcPts.size(); i++ ) {
					if( length(pt-srcPts[i])<6 )
						underPt = i;
				}
				if( underPt!= oldPt ) {
					JGL2::Widget::redraw();
				}
			}break;
			case JGL2::event_t::PUSH : {
				if( underPt>=0 )
					ptOffset = srcPts[underPt]-pt;
			}break;
			case JGL2::event_t::DRAG : {
				if( underPt>=0 ) {
					srcPts[underPt] = pt+ptOffset;
					updateCurve( curveType, srcPts, closed );
					redraw();
				}
			}break;
		}
		return true;
	}
	int underPt = -1;
	vec2 ptOffset = vec2(0);
};


CurveWidget* curveWidget;


void curveTypeCallback(JGL2::Widget* w, void* ud ) {
	curveType = ((JGL2::Options*)w)->value();
//	if (curveType == NATURAL_CLOSED) closed = true;
//	else closed = false;
	updateCurve(curveType, srcPts, closed);
	curveWidget->redraw();
}

void drawTypeCallback(JGL2::Widget* w, void* ud ) {
	drawType = ((JGL2::Options*)w)->value();
	curveWidget->redraw();
}

int main(int argc, const char * argv[]) {
	Window* window = new Window(640,480,"Curves");
	window->alignment(align_t::ALL);
	
	Aligner* aligner = new JGL2::Aligner(0,0,window->w(), window->h());
	aligner->type(Aligner::direction_t::VERTICAL);
	aligner->alignment(align_t::ALL);
	
	Toolbar* toolbar = new Toolbar(0,0,window->w(), _size_toolbar_height() );
	Options* curveTypes = new Options(0,0,200,_size_button_height() );
	curveTypes->add("Linear");
	curveTypes->add("Lagrangian");
	curveTypes->add("Bezier");
	curveTypes->add("Catmull");
	curveTypes->add("Natural");
	curveTypes->add("Bspline");
	curveTypes->value(curveType);
	curveTypes->callback( curveTypeCallback );
	Options* drawType = new Options(0,0,200,_size_button_height() );
	drawType->add("Lines");
	drawType->add("Dots");
	drawType->value(::drawType);
	drawType->callback( drawTypeCallback );
	toolbar->end();
	
	curveWidget = new CurveWidget(0,0,window->w(), window->h()-toolbar->h());
	aligner->resizable( curveWidget );
	aligner->end();
	
	for( auto i=0; i<9; i++ ) {
		srcPts.push_back(vec2((i*0.1+0.1)*curveWidget->w(),curveWidget->h()/2));
	}
	updateCurve(curveType,srcPts,closed);
	
	window->show();
	_JGL::run();
	
	return 0;
}
