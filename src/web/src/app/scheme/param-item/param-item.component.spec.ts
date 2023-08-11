import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { ParamItemComponent } from './param-item.component';

describe('ParamItemComponent', () => {
  let component: ParamItemComponent;
  let fixture: ComponentFixture<ParamItemComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ ParamItemComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ParamItemComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
