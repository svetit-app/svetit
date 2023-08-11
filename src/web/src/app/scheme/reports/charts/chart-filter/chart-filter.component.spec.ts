import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { ChartFilterComponent } from './chart-filter.component';

describe('ChartFilterComponent', () => {
  let component: ChartFilterComponent;
  let fixture: ComponentFixture<ChartFilterComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ ChartFilterComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ChartFilterComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
